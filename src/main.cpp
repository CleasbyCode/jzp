// jzp v1.2 for X/Twitter. Created by Nicholas Cleasby (@CleasbyCode) 14/07/2023.
//
// Compile program (Linux): $ g++ main.cpp -O2 -s -o jzp
// Run program: ./jzp

#include "jzp.h"

int main(int argc, char** argv) {
	if (argc == 2 && std::string(argv[1]) == "--info") {
		displayInfo();
	} else if (argc == 3) { 
		bool isFileCheckSuccess = false;
		
		const std::regex REG_EXP("(\\.[a-zA-Z_0-9\\.\\\\\\s\\-\\/]+)?[a-zA-Z_0-9\\.\\\\\\s\\-\\/]+?(\\.[a-zA-Z0-9]+)?");
		
		const std::string
			IMAGE_FILENAME = argv[1],
			DATA_FILENAME = argv[2];

		std::string image_file_extension = IMAGE_FILENAME.length() > 3 ? IMAGE_FILENAME.substr(IMAGE_FILENAME.length() - 4) : IMAGE_FILENAME;

		image_file_extension = image_file_extension == "jpeg" || image_file_extension == "jfif" ? ".jpg" : image_file_extension;

		if (image_file_extension != ".jpg") {
			std::cerr << "\nFile Type Error: Invalid file extension. Expecting only \"jpg\".\n\n";
		} else if (!regex_match(IMAGE_FILENAME, REG_EXP) || !regex_match(DATA_FILENAME, REG_EXP)) {
			std::cerr << "\nInvalid Input Error: Characters not supported by this program found within filename arguments.\n\n";
		} else if (!std::filesystem::exists(IMAGE_FILENAME) || !std::filesystem::exists(DATA_FILENAME) || !std::filesystem::is_regular_file(DATA_FILENAME)) {
			std::cerr << (!std::filesystem::exists(IMAGE_FILENAME) 
				? "\nImage"
				: "\nData") 
			<< " File Error: File not found. Check the filename and try again.\n\n";
		} else {
			isFileCheckSuccess = true;
		}
		isFileCheckSuccess ? startJzp(IMAGE_FILENAME, DATA_FILENAME) : std::exit(EXIT_FAILURE);	  	
	} else {
		std::cout << "\nUsage: jzp <cover_image> <data_file>\n\t\bjzp --info\n\n";
	}
}
