#include <vector>

#include "slm_target/utils.hpp"

using std::vector;

void printtime(const char* str, double starttime, double endtime) {
	printf("%s%3f seconds\n", str, endtime - starttime);
}

ParseData::ParseData()
{
	std::cout << "Class: Data read \n";
}
void ParseData::readNumberCSV(const string filename, std::vector<float>& NumberVect)
{

	std::unique_ptr<std::ifstream, std::function<void(std::ifstream*)> > pLVifstream(
		new std::ifstream(filename, std::ifstream::in),
		[](std::ifstream* fp) {fp->close(); });

	string row_line;
	if (pLVifstream->is_open()) {
		while (std::getline(*pLVifstream, row_line)) {
			std::istringstream input;
			input.str(row_line);

			for (std::string line; std::getline(input, line, ' '); ) {
				NumberVect.push_back(std::stof(line));
			}

		}
		pLVifstream->close();
	}
	else
	{
		std::cerr << " [ERROR] NumberCSV File not opening " << std::endl;
	}

	// print the value of the vector
}
ParseData::ParseData(const string filename, std::vector<float>& adjMat) :_pifstream(new std::ifstream(filename, std::ifstream::in), [](std::ifstream* fp) {fp->close(); })
{
	int j = 0;
	string row_line;
	if (_pifstream->is_open()) {

		while (std::getline(*_pifstream, row_line)) {
			if (j == 0)
				readDataDim(row_line, adjMat);
			else
			{
				if (j <= _data_dims.at(1) && j != 0)
					readData(row_line, adjMat);
			}
			//std::cout << "Each row "<< filename << " " << row_line << std::endl;
			j++;
		}
		_pifstream->close();
	}
	else
	{
		std::cerr << "File not opening " << std::endl;
	}
}

void ParseData::readLinearValues(const string filename, std::vector<float>& linearVect)
{

	std::unique_ptr<std::ifstream, std::function<void(std::ifstream*)> > pLVifstream(
		new std::ifstream(filename, std::ifstream::in),
		[](std::ifstream* fp) {fp->close(); });

	string row_line;
	if (pLVifstream->is_open()) {
		while (std::getline(*pLVifstream, row_line)) {
			std::istringstream input;
			input.str(row_line);

			for (std::string line; std::getline(input, line, ' '); ) {
				linearVect.push_back(std::stof(line));
			}

		}
		pLVifstream->close();
	}
	else
	{
		std::cerr << " [ERROR] Linear File not opening " << std::endl;
	}

	// print the value of the vector
}

void ParseData::readDataDim(string data, std::vector<float>& adjMat)
{
	std::istringstream input;
	input.str(data);

	for (std::string line; std::getline(input, line, ' '); ) {
		//std::cout << line << " *****  Dimensions Verrtices and Edges **** " << std::endl;
		_data_dims.push_back(std::stoi(line));
		//* std::cout << std::stoi(line) << "*****  Dimensions Verrtices and Edges **** " << std::endl;
		//std::this_thread::sleep_for(std::chrono::seconds(1));

	}

	adjMat.resize( (size_t)_data_dims[0] * _data_dims[0]);

}
void ParseData::readData(string data, std::vector<float>& adjMat)
{

	std::istringstream input;
	input.str(data);
	std::vector<std::string> line_data;//@R std::vector<float> line_data;
	for (std::string line; std::getline(input, line, ' '); ) {
		//std::cout << line << std::endl;
		line_data.push_back(line);
	}
	if (line_data.size() == 3)
	{
		//std::cout << line_data.size() << std::endl;
	  //std::cout << line_data.at(0) <<  " " << line_data.at(1) << " " << line_data.at(2) << std::endl;
		//std::this_thread::sleep_for(std::chrono::seconds(1));
		unsigned int first_entry = std::stoi(line_data.at(0));
		unsigned int sec_entry = std::stoi(line_data.at(1));


		adjMat[((size_t)_data_dims.at(0) * (first_entry - 1)) + (sec_entry - 1)] = stof(line_data.at(2));
		adjMat[((size_t)_data_dims.at(0) * (sec_entry - 1)) + (first_entry - 1)] = stof(line_data.at(2));
		//std::cout << adjMat[_data_dims.at(0)*(line_data.at(0) - 1) + (line_data.at(1) - 1)] << std::endl;
	
	}
}

std::vector<unsigned int> ParseData::getDataDims() const {
	return _data_dims;
}// Parse class ends