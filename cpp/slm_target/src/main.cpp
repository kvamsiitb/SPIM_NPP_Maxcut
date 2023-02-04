/************************ \cond COPYRIGHT *****************************
 *                                                                    *
 * Developed by N Krishna Vamsi for Spatial Photonic Ising Machine    *
 *                                                                    *
 **************************** \endcond ********************************/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <thread>
#include <chrono>

#include <cpp/holoeye_slmdisplaysdk.hpp>
#include <pylon/PylonIncludes.h>
#include <pylon/BaslerUniversalInstantCamera.h>
#include <slm_target/getopt.h>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/stitching.hpp"
#include "opencv2/core/core.hpp"

using namespace holoeye;
#include "cpp/show_slm_preview.h"
#include <slm_target\utils.hpp>

using dataHoloType = float;
using HoloeyeType = field<dataHoloType>;
#define ASSERT(condition) { if(!(condition)){ std::cerr << "ASSERT FAILED: " << #condition << " @ " << __FILE__ << " (" << __LINE__ << ")" << std::endl; } }

// Namespace for using pylon objects.
using namespace Pylon;

// Namespace for using cout.
//using namespace std;
using namespace cv;
// Number of images to be grabbed.
static const uint32_t c_countOfImagesToGrab = 1;

// Namespace for using GenApi objects.
using namespace GenApi;

using std::cout;
using std::vector;
using std::pair;
using std::shared_ptr;
using std::endl;
vector<pair< unsigned int, unsigned int> > SpinTuple(pair<unsigned int, unsigned int> shape
	, unsigned int bin_size)
{
	vector<pair< unsigned int, unsigned int> > lattice_val;
	for (unsigned int i = 0; i < shape.first / bin_size; i++) 
	{
		for (unsigned int j = 0; j < shape.second / bin_size; j++) 
		{
			lattice_val.push_back(std::make_pair(i, j));
		}
	}
	return lattice_val;
}

vector<double> exp_decay(uint32_t num_iters, double initial_temp, double final_temp, double  Critical_temp) // T_c = 2(N - 1) where N = number of spins
{
	vector<double> temperture_list;

	double A = initial_temp - final_temp;
	double B = final_temp;
	double alpha = (-2.0 / num_iters)*log((Critical_temp - final_temp) / (initial_temp - final_temp));
	//print("alpha ", alpha)
	for (int i = 0; i < num_iters; i++) 
	{
		temperture_list.push_back(A*exp(-alpha*i) + B);
	}
	return temperture_list;
}

vector<double> create_beta_schedule_linear(uint32_t num_sweeps, double beta_start, double beta_end)
{
	vector<double> beta_schedule;
	double beta_max;
	if (beta_end == -1)
		beta_max = (1 / 1000) * beta_start;//  here temperature will be zero when beta_max is 1000.f
	else
		beta_max = beta_end;
	double diff = (beta_start - beta_max) / (num_sweeps - 1);// A.P 3.28 - 0.01 inverse value increa final decrease
	for (int i = 0; i < num_sweeps; i++)
	{
		double val = beta_start - (i) * diff;
		beta_schedule.push_back( val );
	}

	return beta_schedule;
}

void DisplayCheckerBoardPattern(shared_ptr<HoloeyeType>& phaseData, int dataWidth, int dataHeight, pair< int, int> area, int outer_bins)
{
	// Initial it to zero
	for (int y = 0; y < dataHeight; ++y)
	{
		dataHoloType* row = phaseData->row(y);
		for (int x = 0; x < dataWidth; ++x)
		{
			row[x] = (float)0.0f;
		}
	}

	// Checkboard pattern of 16 bins
	int sideHeight = (dataHeight - area.first) / 2;
	int sideWidth = (dataWidth - area.second) / 2;
	// number of boxes 
	for (int y = 0; y < area.first / outer_bins; ++y)
	{
		for (int x = 0; x < area.second / outer_bins; ++x)
		{
			for (int k = 0; k < outer_bins; ++k)
			{
				dataHoloType* row = phaseData->row(y * outer_bins + k + sideHeight);
				for (int l = 0; l < outer_bins; ++l)
					row[x * outer_bins + l + sideWidth] = HOLOEYE_PIF * ((x + y) % 2);
			}
		}
	}
}
/*
1 buffer : If fails revert the data by remembering the index
*/
void InitialSLMLattice(shared_ptr<HoloeyeType>& phaseData, int dataWidth, int dataHeight, 
	std::vector<float>& numbVec, std::vector<float>& isingSpins, pair<int, int> active_area, int bins)
{
	// Checkboard pattern of 8 bins
	//pair<int, int> active_area = { 512, 512 };
	int sideHeight = (dataHeight - active_area.first) / 2;
	int sideWidth = (dataWidth - active_area.second) / 2;

	vector< vector< float > > check;
	check.resize(active_area.first);
	for (int ii = 0; ii < active_area.first; ii++)
		check[ii].resize(active_area.second);
	
	int checkBin = 2;
	for (int y = 0; y < active_area.first / checkBin; ++y)
	{
		for (int x = 0; x < active_area.second / checkBin; ++x)
		{
			for (int k = 0; k < checkBin; ++k)
			{
				for (int l = 0; l < checkBin; ++l)
					check[y * checkBin + k][x * checkBin + l] = pow(-1.f, (x + y));
			}
		}
	}

	//cout << check[1][1] << check[131][131] << endl;
	for (int y = 0; y < active_area.first / bins; ++y)
	{
		for (int x = 0; x < active_area.second / bins; ++x)
		{
			for (int k = 0; k < bins; ++k)
			{
				dataHoloType* row = phaseData->row(y * bins + k + sideHeight);
				// (-1)^j cos-1 Em
				for (int l = 0; l < bins; ++l)
					row[x * bins + l + sideWidth] =
							(isingSpins[y * (active_area.first / bins) + x] + 1) * HOLOEYE_PIF / 2 + HOLOEYE_PIF / 2 +
												numbVec[y * (active_area.first / bins) + x] * check[y * bins + k][x * bins + l];
					
			}
		}
	}
}

void FLipLattice(shared_ptr<HoloeyeType> phaseData, int dataWidth, int dataHeight, std::vector<float>& isingSpins,
	pair<int, int> active_area, int bins, vector<pair< unsigned int, unsigned int> >  spinLatticePts, vector<unsigned int> selLatticeIndex)
{
	
	int sideHeight = (dataHeight - active_area.first) / 2;
	int sideWidth = (dataWidth - active_area.second) / 2;
	
	for (unsigned int sel_spin = 0; sel_spin < selLatticeIndex.size(); ++sel_spin)
	{
		unsigned int sel_Index = selLatticeIndex[sel_spin];
		int y = spinLatticePts.at(sel_Index).first;
		int x = spinLatticePts.at(sel_Index).second;

		for (int k = 0; k < bins; ++k)
		{
			
			dataHoloType* row = phaseData->row(y * bins + k + sideHeight);
			// (-1)^j cos-1 Em
			for (int l = 0; l < bins; ++l)
			{
				if (isingSpins[y * (active_area.first / bins) + x] == 1.f)
				{
					row[x * bins + l + sideWidth] -= HOLOEYE_PIF;
					//cout <<  "isingSpins == 1.f" << endl;
				}
				else if (isingSpins[y * (active_area.first / bins) + x] == -1.f)
				{
					row[x * bins + l + sideWidth] += HOLOEYE_PIF;
					//cout << "isingSpins == -1.f" << endl;
				}
			}
			
		}
		isingSpins[y * (active_area.first / bins) + x] *= -1.f;
	}
}


void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
}
class BaslerCamera {
public:
	~BaslerCamera()
	{
		 //delete _camera;
	}

	/*
	* Open Pylon viewer -> Feature All -> Image Format Control -> Change Width, Height, OffsetX, OffsetY
	*/
	BaslerCamera(int timeoutMs = 5000, int width = 1920, int height= 1080, int offsetx = 64, int offsety = 4, float exposure_time = 1100.0) 
	{
		//
		/*
		* First create _camera object
		* open camera object 
		* set exposure time and all the other seting like width, height, offset
		*/
		
		_camera.Attach(CTlFactory::GetInstance().CreateFirstDevice());
		// Open the camera for accessing the parameters.
		_camera.Open();
		INodeMap& nodemap = _camera.GetNodeMap();

		// Get camera device information.
		cout << "Camera Device Information" << endl
			<< "=========================" << endl;
		cout << "Vendor           : "
			<< CStringParameter(nodemap, "DeviceVendorName").GetValue() << endl;
		cout << "Model            : "
			<< CStringParameter(nodemap, "DeviceModelName").GetValue() << endl;
		cout << "Firmware version : "
			<< CStringParameter(nodemap, "DeviceFirmwareVersion").GetValue() << endl << endl;

		// Camera settings.
		cout << "Camera Device Settings" << endl
			<< "======================" << endl;
		_timeoutMs = timeoutMs;

		// Set the AOI:

		// Get the integer nodes describing the AOI.
		CIntegerParameter offsetX(nodemap, "OffsetX");
		CIntegerParameter offsetY(nodemap, "OffsetY");
		CIntegerParameter width1(nodemap, "Width");
		CIntegerParameter height1(nodemap, "Height");
		cout << "Exposure_time           : 0" << endl;
		CIntegerParameter Exposure_time(nodemap, "ExposureTime");
		// On some cameras, the offsets are read-only.
		// Therefore, we must use "Try" functions that only perform the action
		// when parameters are writable. Otherwise, we would get an exception.
		offsetX.TrySetToMinimum();
		offsetY.TrySetToMinimum();

		cout << "Exposure_time           : 1" << endl;
		// Some properties have restrictions.
		// We use API functions that automatically perform value corrections.
		// Alternatively, you can use GetInc() / GetMin() / GetMax() to make sure you set a valid value.
		width1.SetValue(width, IntegerValueCorrection_Nearest);
		height1.SetValue(height, IntegerValueCorrection_Nearest);
		offsetX.SetValue(offsetx, IntegerValueCorrection_Nearest);
		offsetY.SetValue(offsety, IntegerValueCorrection_Nearest);
		if (_camera.ExposureTime.IsValid())
		{
			// We need the integer representation because the GUI controls can only use integer values.
			// If it doesn't exist, return an empty parameter.
			_camera.ExposureTime.SetValue(exposure_time);
		}

		cout << "OffsetX          : " << offsetX.GetValue() << endl;
		cout << "OffsetY          : " << offsetY.GetValue() << endl;
		cout << "Width            : " << width1.GetValue() << endl;
		cout << "Height           : " << height1.GetValue() << endl;
		cout << "Exposure_time           : " << _camera.ExposureTime.GetValue() << endl;

		// Access the PixelFormat enumeration type node.
		CEnumParameter pixelFormat(nodemap, "PixelFormat");

		// Remember the current pixel format.
		String_t oldPixelFormat = pixelFormat.GetValue();
		cout << "Old PixelFormat  : " << oldPixelFormat << endl;

		// Set the pixel format to Mono8 if available.
		if (pixelFormat.CanSetValue("Mono8"))
		{
			pixelFormat.SetValue("Mono8");
			cout << "New PixelFormat  : " << pixelFormat.GetValue() << endl;
		}

		/*
		// Set the new gain to 50% ->  Min + ((Max-Min) / 2).
		//
		// Note: Some newer camera models may have auto functions enabled.
		//       To be able to set the gain value to a specific value
		//       the Gain Auto function must be disabled first.
		// Access the enumeration type node GainAuto.
		// We use a "Try" function that only performs the action if the parameter is writable.
		CEnumParameter gainAuto(nodemap, "GainAuto");
		gainAuto.TrySetValue("Off");


		// Check to see which Standard Feature Naming Convention (SFNC) is used by the camera device.
		if (_camera.GetSfncVersion() >= Sfnc_2_0_0)
		{
			// Access the Gain float type node. This node is available for USB camera devices.
			// USB camera devices are compliant to SFNC version 2.0.
			CFloatParameter gain(nodemap, "Gain");
			gain.SetValuePercentOfRange(50.0);
			cout << "Gain (50%)       : " << gain.GetValue() << " (Min: " << gain.GetMin() << "; Max: " << gain.GetMax() << ")" << endl;
		}
		else
		{
			// Access the GainRaw integer type node. This node is available for IIDC 1394 and GigE camera devices.
			CIntegerParameter gainRaw(nodemap, "GainRaw");
			gainRaw.SetValuePercentOfRange(50.0);
			cout << "Gain (50%)       : " << gainRaw.GetValue() << " (Min: " << gainRaw.GetMin() << "; Max: " << gainRaw.GetMax() << "; Inc: " << gainRaw.GetInc() << ")" << endl;
		}
		*/


	}
	// Grab_UsingExposureEndEvent
	void checkImage()
	{
		//grab one image
		_camera.StartGrabbing(1, GrabStrategy_OneByOne, GrabLoop_ProvidedByUser);
		//grab is stopped automatically due to maxImages = 1
		bool hasRetrieved = _camera.RetrieveResult(_timeoutMs, _ptrGrabResult, TimeoutHandling_ThrowException) && _ptrGrabResult->GrabSucceeded();

		cout << "1. hasRetrieved: " << std::boolalpha << hasRetrieved << endl;
		cout << "SizeX: " << _ptrGrabResult->GetWidth() << endl;
		cout << "SizeY: " << _ptrGrabResult->GetHeight() << endl;

		if (_ptrGrabResult->GrabSucceeded())
		{
			Mat openCvImage = Mat(_ptrGrabResult->GetHeight(), _ptrGrabResult->GetWidth(), CV_8UC1, (void*)_ptrGrabResult->GetBuffer());
			//Create a window
			namedWindow("My Window", 1);
			//set the callback function for any mouse event
			setMouseCallback("My Window", CallBackFunc, NULL);
			//show the image
			imshow("My Window", openCvImage);
			waitKey(0);
		}

		/*
		cout << "Provide _xin, _yin, _xout, _yout in this order: " <<  endl;
		
		std::cin >> _xin;
		std::cin >> _yin;
		std::cin >> _xout;
		std::cin >> _yout;
		
		_heightFinImg = _xout - _xin;
		_widthFinImg = _yout - _yin;
		_areaFinImg = _heightFinImg * _widthFinImg;
		*/
	}

	void openBaslerCamera()
	{
		_camera.Open();
	}
	// Grab_UsingExposureEndEvent
	float collectSingleImageNEnergy()
	{
		float energy = 0.f;

		//grab one image
		_camera.StartGrabbing(1, GrabStrategy_OneByOne, GrabLoop_ProvidedByUser);
		//grab is stopped automatically due to maxImages = 1
		bool hasRetrieved = _camera.RetrieveResult(_timeoutMs, _ptrGrabResult, TimeoutHandling_ThrowException) && _ptrGrabResult->GrabSucceeded();

		/*
		cout << "1. hasRetrieved: " << std::boolalpha << hasRetrieved << endl;
		cout << "SizeX: " << _ptrGrabResult->GetWidth() << endl;
		cout << "SizeY: " << _ptrGrabResult->GetHeight() << endl;
		// Image grabbed successfully?
		cout << "2. here i am" << endl;
		*/
		if (_ptrGrabResult->GrabSucceeded())
		{
			uint8_t* pImageBuffer = (uint8_t*)_ptrGrabResult->GetBuffer();
			int sum = 0;
			for (int y = 0; y < _ptrGrabResult->GetHeight(); ++y)// (int y = _yin; y < _yout; ++y)
				for (int x = 0; x < _ptrGrabResult->GetWidth(); ++x)
					sum += (int)pImageBuffer[y * _ptrGrabResult->GetWidth() + x];
			energy = static_cast<float>(sum) / static_cast<float>(_ptrGrabResult->GetHeight() * _ptrGrabResult->GetWidth());
			//cout << "Energy: " << energy << " sum: " << sum << endl;
		}
		else
		{
			energy = -1.f;//cout << "Error: " << std::hex << _ptrGrabResult->GetErrorCode() << std::dec << " " << _ptrGrabResult->GetErrorDescription() << endl;
		}

		return energy; // energy = -1 if hasRetrieved = false then rerun the loop
	}

	void closeBaslerCamera()
	{
		_camera.Close();
	}
private:
	// C:\Program Files\Basler\pylon 6\Development\Samples\C++\GUI_MFCMultiCam\GuiCamera.cpp
	CBaslerUniversalInstantCamera _camera;

	//CInstantCamera _camera;
	// https://docs.baslerweb.com/auto-function-roi#sample-code
	// CIntegerParameter m_exposureTime;
	CGrabResultPtr _ptrGrabResult;
	int _timeoutMs;
	int _xin, _yin, _xout, _yout, _areaFinImg, _widthFinImg, _heightFinImg;
};

void CalculateHamiltonian(vector<vector<float>>& Ham, vector<float> numberVect)
{
	Ham.resize(numberVect.size());

	for (int i = 0; i < numberVect.size(); i++)
	{
		Ham[i].resize(numberVect.size());
	}

	for (int i = 0; i < numberVect.size(); i++)
	{
		for (int j = 0; j < numberVect.size(); j++)
		{
			Ham[i][j] = (-1.f) * numberVect[i] * numberVect[j];
		}
	}
}

float CalculateEnergy(vector<vector<float>> Ham, vector<float> numberVect, vector<float> isingSpins)
{
	float energy = 0.f;

	for (int i = 0; i < numberVect.size(); i++)
	{
		for (int j = 0; j < numberVect.size(); j++)
		{
			energy += (-1.f) * Ham[i][j] * isingSpins[i] * isingSpins[j];
		}
	}

	return energy;
}
float DiffNumbers(vector<float> numberVect, vector<float> isingSpins)
{
	ASSERT(numberVect.size() == isingSpins.size() );

	float diff = 0.f;
	for (int i = 0; i < numberVect.size(); i++)
	{
		diff += numberVect.at(i) * isingSpins.at(i);
	}
	//diff = abs(diff);
	return diff;
}

void WriteToFile(std::ofstream& outputFile, float x)
{
	outputFile << x << endl;
}

static void usage(const char* pname) {

	const char* bname = nullptr;//@R = rindex(pname, '/');

	fprintf(stdout,
		"Usage: %s [options]\n"
		"options:\n"
		"\t-a|--filename  <String>\n"
		"\t\tfilename containing numbers\n"
		"\n"
		"\t-x|--start temperature <FLOAT>\n"
		"\t\t \n"
		"\n"
		"\t-y|--stop temperature <FLOAT>\n"
		"\t\tnumber of lattice columns\n"
		"\n"
		"\t-n|--num_temps <INT>\n"
		"\t\tnumber of iterations\n"
		"\n"
		"\t-n|--sweeps_per_beta <INT>\n"
		"\t\tnumber of sweep per temperature\n"
		"\n"
		"\t-s|--seed <SEED>\n"
		"\t\tfix the starting point\n"
		"\n"
		"\t-s|--debug \n"
		"\t\t Print the final lattice value and shows avg magnetization at every temperature\n"
		"\n"
		"\t-o|--write-lattice\n"
		"\t\twrite final lattice configuration to file\n\n",
		bname);
	exit(EXIT_SUCCESS);
}
#define MYDEBU 1


// ./SPIM.exe -a numbers_16.csv -x 530 -y 0.01 -n 1000 -m 1 -1 1015 -2 542 -w 5 -h 5 -e 4000 -m 1
// # x1 1015 y1 542 x2 1020 y2 547 w 5 h 5 -w (x1-x2) -h (y2-y1)
int main(int argc, char* argv[])
{
	vector<float> energies;
	vector<float> final_spins;
	energies.push_back(255.0);

	vector<float> hamEnergies;
	vector<float> fidelities;

	std::string filename = "";//argv[1]
	std::string linear_file = "";

	float start_temp = 600.f;
	float stop_temp = 0.01f;
	float critical_temp = -1.f; // 2*(N-1) where N is number of Ising spins
	unsigned long long seed = ((GetCurrentProcessId() * rand()) & 0x7FFFFFFFF);

	unsigned int num_temps = 1000; //atoi(argv[2]);

	bool write = false;
	bool debug = false;

	int CamtimeoutMs = 5000;// 15000
	int width = 1920;
	int height = 1080;
	int offsetx = 64;
	int offsety = 4;
	float exposure_value = 10000.0;// reduce for faster data transfer
	int out_bi = 16;
	int are = 1024;
	int bi = 8;
	int act_are = 512;
	int NUM_SPINS_FLIP = 1;
	std::cout << "Start parsing the file containing numbers" << std::endl;

	while (1) {
		static struct option long_options[] = {
			{     "Number_filename", required_argument, 0, 'a'},
			{     "start_temp", required_argument, 0, 'x'},
			{     "stop_temp", required_argument, 0, 'y'},
			{          "seed", required_argument, 0, 's'},
			{        "num_temps", required_argument, 0, 'n'},
			{ "NUM_SPINS_FLIP", required_argument, 0, 'm'},
			{  "CamTimeOut",       required_argument, 0, 't'},
			{  "width",       required_argument, 0, 'w'},
			{  "height",       required_argument, 0, 'h'},
			{  "offsetx",       required_argument, 0, '1'},
			{  "offsety",       required_argument, 0, '2'},
			{  "exposure_value",       required_argument, 0, 'e'},
			{  "outer_bin",       required_argument, 0, '3'},
			{  "area",       required_argument, 0, '4'},
			{  "bin",       required_argument, 0, '5'},
			{  "active_area",       required_argument, 0, '6'},
			{ "write-lattice",       no_argument, 0, 'o'},
			{          "debug",       no_argument, 0, 'd'},
			{          "help",       no_argument, 0, 'z'},
			{               0,                 0, 0,   0}
		};

		int option_index = 0;
		int ch = getopt_long(argc, argv, "a:x:y:s:n:m:t:w:h:1:2:e:3:4:5:6:odz", long_options, &option_index);
		if (ch == -1) break;

		switch (ch) {
		case 0:
			break;
		case 'a':
			filename = (optarg); break;
		case 'x':
			start_temp = atof(optarg); break;
		case 'y':
			stop_temp = atof(optarg); break;
		case 's':
			seed = atoll(optarg);
			break;
		case 'n':
			num_temps = atoi(optarg); break;
		case 'm':
			NUM_SPINS_FLIP = atoi(optarg); break;
		case 'o':
			write = true; break;
		case 'd':
			debug = true; break;
		case 'z':
			usage(argv[0]); break;
		case '?':
			exit(EXIT_FAILURE);
		case 't':
			CamtimeoutMs = atoi(optarg); break;
		case 'w':
			width = atoi(optarg); break;
		case 'h':
			height = atoi(optarg); break;
		case '1':
			offsetx = atoi(optarg); break;
		case '2':
			offsety = atoi(optarg); break;
		case 'e':
			exposure_value = atof(optarg); break;
		case '3':
			out_bi = atoi(optarg); break;
		case '4':
			are = atoi(optarg); break;
		case '5':
			bi = atoi(optarg); break;
		case '6':
			act_are = atoi(optarg); break;
		default:
			fprintf(stderr, "unknown option: %c\n", ch);
			exit(EXIT_FAILURE);
		}
	}

	HOLOEYE_UNUSED(argc);
	HOLOEYE_UNUSED(argv);

		//
		std::string hamEnergy_filename = "hamEnergy_";
		std::string fidelity_filename = "fidelity_";
		std::string spins_filename = "spins_";
		std::string intensity_filename = "intensity_";
		std::string Temps_filename = "Temps_";
		{
			// Find position of '_' using find()
			int pos = filename.find_last_of("_");
			// Copy substring after pos
			std::string sub = filename.substr(pos + 1);
			hamEnergy_filename += sub;
			fidelity_filename += sub;
			spins_filename += sub;
			intensity_filename += sub;
			Temps_filename += sub;
		}

		std::ofstream hamEnergy_outputFile;
		hamEnergy_outputFile.open(hamEnergy_filename.c_str());

		std::ofstream fidelity_outputFile;
		fidelity_outputFile.open(fidelity_filename.c_str());

		std::ofstream spins_outputFile;
		spins_outputFile.open(spins_filename.c_str());

		std::ofstream intensity_outputFile;
		intensity_outputFile.open(intensity_filename.c_str());

		std::ofstream Temps_outputFile;
		Temps_outputFile.open(Temps_filename.c_str());

		ParseData parseObj = ParseData();

		vector<float> numberVect;
		parseObj.readNumberCSV(filename, numberVect);
		float maxNum = *std::max_element(numberVect.begin(), numberVect.end());

		vector<float> normalNumberVect(numberVect);
		for (int i = 0; i < normalNumberVect.size(); i++)
			normalNumberVect[i] = acos(normalNumberVect.at(i) / maxNum);
		cout << "Parse the numbers from csv file" << endl;
		// Check if the installed SDK supports the required API version
		if (!heds_requires_version(3, false))
			return 1;
		vector<vector<float>> Hamiltonian;
		CalculateHamiltonian(Hamiltonian, numberVect);
		int exitCode = 0;


#if MYDEBU
		// Before using any pylon methods, the pylon runtime must be initialized.
		PylonInitialize();
		// The exit code of the sample application.

		BaslerCamera baslerCamera(CamtimeoutMs, width, height, offsetx, offsety, exposure_value); // = BaslerCamera(CamtimeoutMs, width, height, offsetx, offsety, exposure_value);
		

		baslerCamera.openBaslerCamera();

		// Detect SLMs and open a window on the selected SLM:
		heds_instance slm;
		heds_errorcode error = slm.open();
		if (error != HEDSERR_NoError)
		{
			std::cerr << "ERROR: " << heds_error_string_ascii(error) << std::endl;
			return error;
		}

		// Open the SLM preview window in "Fit" mode:
		// Please adapt the file show_slm_preview.h if preview window
		// is not at the right position or even not visible.
		// The additional flag HEDSSLMPF_ShowZernikeRadius presses the button to
		// show the Zernike radius visualization in preview window from code.
		
		error = show_slm_preview(0.0);
		if (error != HEDSERR_NoError)
		{
			std::cerr << "ERROR: " << heds_error_string_ascii(error) << std::endl;
			return error;
		}

		// pre-calc. helper variables:
		const float phaseModulation = 2.0f * HOLOEYE_PIF;
		const int dataWidth = heds_slm_width_px();
		const int dataHeight = heds_slm_height_px();

		// Reserve memory for the phase data matrix.
		// Use data type single to optimize performance:
		auto phaseData = HoloeyeType::create(dataWidth, dataHeight);
		
		// phaseData.refreshrate()
		std::cout << "dataWidth  = " << dataWidth << std::endl;
		std::cout << "dataHeight = " << dataHeight << std::endl;

		// Display checkerboard of size {1024, 1024}  and bins {16, 16}
		int outer_bin = out_bi;//pow(2, 4); 
		pair< int, int> area = { are, are };
		DisplayCheckerBoardPattern(phaseData, dataWidth, dataHeight, area, outer_bin);
		// Show phase data on SLM:
		error = heds_show_phasevalues(phaseData); //heds_show_phasevalues(phaseData, HEDSSHF_PresentAutomatic, phaseModulation);
		if (error != HEDSERR_NoError)
		{
			std::cerr << "ERROR: " << heds_error_string_ascii(error) << std::endl;
			return error;
		}
		

		std::this_thread::sleep_for(std::chrono::milliseconds(150));
		std::cout << "Do Ur calibration Using Pylon GUI and then press enter" << std::endl;
		char ch = getchar();


		std::vector<float> isingSpins;
		srand(time(0));
		for (int i = 0; i < numberVect.size(); ++i)
			isingSpins.push_back(1.f); //float( 2.f * (rand() % 2) - 1.f) );


 		ASSERT(numberVect.size() == isingSpins.size());
		final_spins.resize(isingSpins.size());

		bi = int(act_are / sqrt(numberVect.size()));
		
		int bin = bi;// pow(2, 3);// pow(2, 7);
		
		pair<int, int> active_area = { act_are, act_are};
		//cout << "1. InitialSLMLattice" << endl;
		InitialSLMLattice(phaseData, dataWidth, dataHeight, normalNumberVect, isingSpins, active_area, bin);

		// Show phase data on SLM:
		error = heds_show_phasevalues(phaseData); // heds_show_phasevalues(phaseData, HEDSSHF_PresentAutomatic, phaseModulation);
		if (error != HEDSERR_NoError)
		{
			std::cerr << "ERROR: " << heds_error_string_ascii(error) << std::endl;
			return error;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(150));
		//std::cout << "See the expected pattern on camera" << std::endl;
		baslerCamera.checkImage();// @R
		if (critical_temp == -1.0)
		{
			std::cout << "[Note] based on the number spin size N the value of critical temperature T_c = 2(N-1) " << std::endl;
			if (critical_temp < start_temp)
			{
				critical_temp = 2.0*(numberVect.size() - 1);
			}
			
			else
			{
				critical_temp = (start_temp + stop_temp) / 2;
			}
			
		}

		vector<double> vecTemps = exp_decay(num_temps,  start_temp, stop_temp, critical_temp);

		// Lattice creation
		vector<pair< unsigned int, unsigned int> >  spinLatticePts = SpinTuple( active_area, bin);
		vector<unsigned int> selLatticeIndex; 
		selLatticeIndex.resize(NUM_SPINS_FLIP, 0);

		float best_diff = std::numeric_limits<float>::infinity();
		auto start = std::chrono::high_resolution_clock::now();
		// Flip lattice
		for (int count = 0; count < vecTemps.size(); )
		{
				for (int i = 0; i < NUM_SPINS_FLIP; i++)
					selLatticeIndex[i] = static_cast<unsigned int>( rand() % spinLatticePts.size() );

				FLipLattice(phaseData, dataWidth, dataHeight, isingSpins, active_area, bin, spinLatticePts, selLatticeIndex);
				// Show phase data on SLM:
				error = heds_show_phasevalues(phaseData); //heds_show_phasevalues(phaseData, HEDSSHF_PresentAutomatic, phaseModulation);
				if (error != HEDSERR_NoError)
				{
					std::cerr << "ERROR: " << heds_error_string_ascii(error) << std::endl;
					return error;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(150));
				try
				{
					float energy = baslerCamera.collectSingleImageNEnergy(); // @R
					//cout << "Energy: " << energy << endl;
					if (energy != -1.f)
					{
						// MH algo
						float delE = energy - energies[energies.size() - 1];
						// sum of Ising spins with numbers for fidelity
						float prob = exp(-1.f * (1 /vecTemps[count]) * delE);
						float acceptance_probability = min((float)1.f, prob);
						// Flip back if not selected in MH iter
						double gen_pro = ((double)rand() / (RAND_MAX));
						if (delE <= 0)
						{
							energies.push_back(energy);
						}
						else if (gen_pro >= acceptance_probability)
						{
							energies.push_back(energy);
						}
						else
						{
							energies.push_back(energies[energies.size() - 1]);
							FLipLattice(phaseData, dataWidth, dataHeight, isingSpins, active_area, bin, spinLatticePts, selLatticeIndex);
						}
						count++;
						float Hamltonian_Energy = CalculateEnergy(Hamiltonian, numberVect, isingSpins);
						// Calculate the Hamiltonian Energy
						hamEnergies.push_back(Hamltonian_Energy);
						// Calculate the fidelity
						float diff = DiffNumbers(numberVect, isingSpins);
						
						fidelities.push_back(diff);
						if (abs(diff) < best_diff)
						{
							best_diff = abs(diff);
							copy(isingSpins.begin(), isingSpins.end(), final_spins.begin());
						}

					}

				}
				catch (const GenericException & e)
				{
					// Error handling.
					std::cerr << "An exception occurred." << endl
						<< e.GetDescription() << endl;
					
					FLipLattice(phaseData, dataWidth, dataHeight, isingSpins, active_area, bin, spinLatticePts, selLatticeIndex);
				}
		}
		auto end = std::chrono::high_resolution_clock::now();

		double duration = (double)std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

		std::cout << "Number of count: " << vecTemps.size() << " elapsed time: " << duration * 1e-6 << " ms\n";
	
		// Wait until the SLM process was closed
		std::cout << "Waiting for SDK process to close. Please close the tray icon to continue ..." << std::endl << std::flush;
		error = heds_utils_wait_until_closed();

		if (error != HEDSERR_NoError)
		{
			std::cerr << "ERROR: " << heds_error_string_ascii(error) << std::endl;

			return error;
		}

		for (int ii = 0; ii < hamEnergies.size(); ii++)
		{
			WriteToFile(hamEnergy_outputFile, hamEnergies[ii]);
			WriteToFile(fidelity_outputFile, fidelities[ii]);
			WriteToFile(intensity_outputFile, energies[ii]);
			WriteToFile(Temps_outputFile, vecTemps[ii]);
		}
		cout << "Best diff: " << best_diff << endl;

		for (int ii = 0; ii < final_spins.size(); ii++)
		{
			WriteToFile(spins_outputFile, final_spins[ii]);
		}
		baslerCamera.closeBaslerCamera();
		// Releases all pylon resources.
		PylonTerminate();
#endif
	
		// Comment the following two lines to disable waiting on exit.
		std::cerr << endl << "Press enter to exit. with best diff: " << best_diff << endl;
		while (std::cin.get() != '\n');

		return exitCode;
	
}


#define CPU_GPU_COMPARISON 0

#if CPU_GPU_COMPARISON

Show the pattern on slm
wait for 50ms 

Parallel (Image capture && \sum n * s)

Bandwidth manager
// https://holoeye.com/wp-content/uploads/Application_Note_SLM-V.59.pdf

// https://sodocumentation.net/cuda/topic/6566/parallel-reduction--e-g--how-to-sum-an-array-#:~:text=The%20simplest%20approach%20to%20parallel,)%20%7B%20int%20idx%20%3D%20threadIdx.

static const int wholeArraySize = 65536000;
static const int blockSize = 1024;
static const int gridSize = 12; //this number is hardware-dependent; usually #SM*2 is a good number.

__global__ void sumCommMultiBlock(const int* gArr, int arraySize, int* gOut) {
	int thIdx = threadIdx.x;
	int gthIdx = thIdx + blockIdx.x * blockSize;
	const int gridSize = blockSize * gridDim.x;
	int sum = 0;
	for (int i = gthIdx; i < arraySize; i += gridSize)
		sum += gArr[i];
	__shared__ int shArr[blockSize];
	shArr[thIdx] = sum;
	__syncthreads();
	for (int size = blockSize / 2; size > 0; size /= 2) { //uniform
		if (thIdx < size)
			shArr[thIdx] += shArr[thIdx + size];
		__syncthreads();
	}
	if (thIdx == 0)
		gOut[blockIdx.x] = shArr[0];
}

__host__ int sumArray(vector<int> arr) {
	int* dev_arr;
	cudaMalloc((void**)&dev_arr, wholeArraySize * sizeof(int));
	cudaMemcpy(dev_arr, arr.data(), wholeArraySize * sizeof(int), cudaMemcpyHostToDevice);

	int out;
	int* dev_out;
	cudaMalloc((void**)&dev_out, sizeof(int) * gridSize);
	auto t0 = std::chrono::high_resolution_clock::now();
	sumCommMultiBlock << <gridSize, blockSize >> > (dev_arr, wholeArraySize, dev_out);
	//dev_out now holds the partial result
	sumCommMultiBlock << < 1, blockSize >> > (dev_out, gridSize, dev_out);
	//dev_out[0] now holds the final result
	cudaDeviceSynchronize();
	auto t1 = std::chrono::high_resolution_clock::now();
	float duration = (float)std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();

	printf("duration for GPU %.6f \n", (duration * 1e-6));//28e-6 // 203e-6

	cudaMemcpy(&out, dev_out, sizeof(int), cudaMemcpyDeviceToHost);
	cudaFree(dev_arr);
	cudaFree(dev_out);
	return out;
}

void main() {
	int sum = 0;
	vector<int> vect(wholeArraySize, 1);
	cout << vect.size() << endl;
	auto t0 = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < wholeArraySize; i++)
	{
		sum += vect[i];
	}
	//this_thread::sleep_for(chrono::milliseconds(20000));

	auto t1 = std::chrono::high_resolution_clock::now();
	float duration = (float)std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();

	printf("duration for CPU %.6f, sum %d \n", (duration * 1e-6), sum);//28e-6 // 203e-6

	int sum1 = sumArray(vect);

	printf("Sum %d\n", sum1);
}

#endif