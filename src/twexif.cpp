// twexif v1.0 for Twitter. Created by Nicholas Cleasby (CleasbyCode) 14/07/2023.

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct jdvStruct {
	std::vector<unsigned char> ImageVec, ProfileVec;
	std::string IMAGE_NAME, ZIP_NAME;
	size_t IMAGE_SIZE{}, ZIP_SIZE{};
};

// Update values, such as Profile size, file sizes, offsets and other values, and write them into the relevant vector index locations.
// Overwrites previous values.

class ValueUpdater {
public:
	void Value(std::vector<unsigned char>& vec, size_t valueInsertIndex, const size_t VALUE, unsigned short bits, bool isBig) {
		if (isBig) {
			while (bits) vec[valueInsertIndex++] = (VALUE >> (bits -= 8)) & 0xff;
		}
		else {
			while (bits) vec[valueInsertIndex--] = (VALUE >> (bits -= 8)) & 0xff;
		}
	}
} *update;


// Open user image & ZIP file. Display error & exit program if any file fails to open.
void openFiles(jdvStruct& jdv);

// Adjust ZIP file offsets now that it's embedded within the JPG image, so that it remains a valid, working ZIP archive.
void fixZipOffset(std::vector<unsigned char>&);

// Write out to file the zip-embedded image file.
void writeOutFile(jdvStruct& jdv);

// Display program infomation
void displayInfo();

int main(int argc, char** argv) {

	jdvStruct jdv;

	if (argc == 2 && std::string(argv[1]) == "--info") {
		displayInfo();
	}
	else if (argc > 2 && argc < 4) { 
		jdv.IMAGE_NAME = argv[1];
		jdv.ZIP_NAME = argv[2];
		openFiles(jdv);
	}
	else {
		std::cout << "\nUsage:\t\btwexif <jpg_image>  <zip_file>\n\t\btwexif --info\n\n";
	}
	return 0;
}

void openFiles(jdvStruct& jdv) {

	const std::string READ_ERR_MSG = "\nRead Error: Unable to open/read file: ";

	std::ifstream
		readImage(jdv.IMAGE_NAME, std::ios::binary),
		readFile(jdv.ZIP_NAME, std::ios::binary);

	if (!readImage || !readFile) {

		// Open file failure, display relevant error message and exit program.
		const std::string ERR_MSG = !readImage ? READ_ERR_MSG + "\"" + jdv.IMAGE_NAME + "\"\n\n" : READ_ERR_MSG + "\"" + jdv.ZIP_NAME + "\"\n\n";

		std::cerr << ERR_MSG;
		std::exit(EXIT_FAILURE);
	}

	// Get size of files.
	readImage.seekg(0, readImage.end),
	readFile.seekg(0, readFile.end);

	// Update file size variables.
	jdv.IMAGE_SIZE = readImage.tellg();
	jdv.ZIP_SIZE = readFile.tellg();

	// Reset read position of files. 
	readImage.seekg(0, readImage.beg),
	readFile.seekg(0, readFile.beg);

	// ZIP file size limit for Twitter.
	// Twitter allows for only one ICC Profile, with a size limit of just 10KB.
	const size_t MAX_FILE_SIZE = 10000;

	if (jdv.ZIP_SIZE > MAX_FILE_SIZE) {
	// File size check failure, display error message and exit program.
		std::cerr << "\nFile Size Error:\n\n  Your ZIP file size [" << jdv.ZIP_SIZE << " Bytes] must not exceed 10KB.\n\n";
		std::exit(EXIT_FAILURE);
	}

	// The first 152 bytes of this vector contains the main (basic) ICC Profile.
	// ZIP file will be inserted & stored at the end of this vector.
	jdv.ProfileVec = {
		0xFF, 0xD8, 0xFF, 0xE0, 0x00, 0x10, 0x4A, 0x46, 0x49, 0x46, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01,
		0x00, 0x01, 0x00, 0x00, 0xFF, 0xE2, 0x0E, 0x8C, 0x49, 0x43, 0x43, 0x5F, 0x50, 0x52, 0x4F, 0x46,
		0x49, 0x4C, 0x45, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x84, 0x20, 0x54, 0x77, 0x45, 0x78, 0x69,
		0x66, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x61, 0x63, 0x73, 0x70, 0x0D, 0x0A,
		0x26, 0x63, 0x6C, 0x73, 0x26, 0x72, 0x65, 0x6E, 0x20, 0x22, 0x25, 0x7E, 0x6E, 0x30, 0x25, 0x7E,
		0x78, 0x30, 0x22, 0x20, 0x74, 0x5F, 0x2E, 0x7A, 0x69, 0x70, 0x26, 0x70, 0x6F, 0x77, 0x65, 0x72,
		0x73, 0x68, 0x65, 0x6C, 0x6C, 0x20, 0x45, 0x78, 0x70, 0x61, 0x6E, 0x64, 0x2D, 0x41, 0x72, 0x63,
		0x68, 0x69, 0x76, 0x65, 0x20, 0x74, 0x5F, 0x2E, 0x7A, 0x69, 0x70, 0x26, 0x72, 0x65, 0x6E, 0x20,
		0x74, 0x5F, 0x2E, 0x7A, 0x69, 0x70, 0x20, 0x74, 0x5F, 0x2E, 0x6A, 0x70, 0x67, 0x26, 0x65, 0x78,
		0x69, 0x74, 0x0D, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

	// Read-in and store JPG image file into vector "ImageVec".
	jdv.ImageVec.resize(jdv.IMAGE_SIZE / sizeof(unsigned char));
	readImage.read((char*)jdv.ImageVec.data(), jdv.IMAGE_SIZE);

	// Read-in and store user's ZIP file at the end of vector "ProfileVec".
	jdv.ProfileVec.resize(jdv.ZIP_SIZE + jdv.ProfileVec.size() / sizeof(unsigned char));
	readFile.read((char*)&jdv.ProfileVec[170], jdv.ZIP_SIZE);

	const std::string
		JPG_SIG = "\xFF\xD8\xFF",	// JPG image signature. 
		ZIP_SIG = "PK\x03\x04",		// ZIP file signature
		JPG_CHECK{ jdv.ImageVec.begin(), jdv.ImageVec.begin() + JPG_SIG.length() },			// Get JPG header from vector. 
		ZIP_CHECK{ jdv.ProfileVec.begin()+170, jdv.ProfileVec.begin() + 170 + ZIP_SIG.length() };	// Get ZIP header from vector. }

	// Make sure we are dealing with valid JPG & ZIP files.
	if (JPG_CHECK != JPG_SIG || ZIP_CHECK != ZIP_SIG) {
			
		const std::string
			HEADER_ERR_MSG = "Format Error: File does not appear to be a valid",
			ERR_MSG = JPG_CHECK != JPG_SIG ? "\nJPG " + HEADER_ERR_MSG + " JPG image.\n\n"
			: "\nZIP " + HEADER_ERR_MSG + " ZIP archive.\n\n";

			// File requirements check failure, display relevant error message and exit program.
		std::cerr << ERR_MSG;
		std::exit(EXIT_FAILURE);
	}

	// Signature for Define Quantization Table(s) 
	const auto DQT_SIG = { 0xFF, 0xDB };

	// Find location in vector "ImageVec" of first DQT index location of the image file.
	const size_t DQT_POS = search(jdv.ImageVec.begin(), jdv.ImageVec.end(), DQT_SIG.begin(), DQT_SIG.end()) - jdv.ImageVec.begin();

	// Erase the first n bytes of the JPG header before this DQT position. We later replace the erased header with the contents of vector "ProfileVec".
	jdv.ImageVec.erase(jdv.ImageVec.begin(), jdv.ImageVec.begin() + DQT_POS);

	unsigned short int
		bits = 16,			// Variable used with the "updateValue" function. Two bytes max for ICC Profile size.
		profileMainBlockSizeIndex = 22;	// "ProfileVec" start index location for the 2 byte block size field.

	const size_t
		VECTOR_SIZE = jdv.ProfileVec.size() - profileMainBlockSizeIndex;	// Get updated size for vector "ProfileVec" after adding user's ZIP file.
		
		// Update Profile block size of vector "ProfileVec".
		update->Value(jdv.ProfileVec, profileMainBlockSizeIndex, VECTOR_SIZE, bits, true);

		// Adjust ZIP file offsets within this vector, as their locations have now changed.
		fixZipOffset(jdv.ProfileVec);

		// Insert contents of vector "ProfileVec" into vector "ImageVec", combining user's ICC Profile + user's ZIP file, with the JPG image.	
		jdv.ImageVec.insert(jdv.ImageVec.begin(), jdv.ProfileVec.begin(), jdv.ProfileVec.end());

		jdv.ZIP_NAME = "twexif_img.jpg";

	writeOutFile(jdv);
}

void fixZipOffset(std::vector<unsigned char>& Vec) {

	const std::string
		START_CENTRAL_SIG = "PK\x01\x02",
		END_CENTRAL_SIG = "PK\x05\x06",
		ZIP_SIG = "PK\x03\x04";

	// Search vector to get index locations for "Start Central Directory" & "End Central Directory".
	const size_t
		START_CENTRAL_INDEX = search(Vec.begin() + 168, Vec.end(), START_CENTRAL_SIG.begin(), START_CENTRAL_SIG.end()) - Vec.begin(),
		END_CENTRAL_INDEX = search(Vec.begin() + START_CENTRAL_INDEX, Vec.end(), END_CENTRAL_SIG.begin(), END_CENTRAL_SIG.end()) - Vec.begin();

	size_t
		zipRecordsIndex = END_CENTRAL_INDEX + 11,	// Index location for ZIP file records value.
		endCentralStartIndex = END_CENTRAL_INDEX + 19,	// Index location for End Central Start offset.
		centralLocalIndex = START_CENTRAL_INDEX - 1,	// Initialise variable to just before Start Central index location.
		newOffset = 168,							// Initialise variable to near location of ZIP file.
		zipRecords = (Vec[zipRecordsIndex] << 8) | Vec[zipRecordsIndex - 1];	// Get ZIP file records value from index location within vector.

	// Search for ZIP file record offsets and update them to their new offset location.
	while (zipRecords--) {
		newOffset = search(Vec.begin() + newOffset + 1, Vec.end(), ZIP_SIG.begin(), ZIP_SIG.end()) - Vec.begin(),
		centralLocalIndex = 45 + search(Vec.begin() + centralLocalIndex, Vec.end(), START_CENTRAL_SIG.begin(), START_CENTRAL_SIG.end()) - Vec.begin();
		update->Value(Vec, centralLocalIndex, newOffset, 32, false);
	}

	// Write updated "Start Central Directory" offset into End Central Directory's "Start Central Directory" index location within vector.
	update->Value(Vec, endCentralStartIndex, START_CENTRAL_INDEX, 32, false);
}

void writeOutFile(jdvStruct& jdv) {

	std::ofstream writeFile(jdv.ZIP_NAME, std::ios::binary);

	if (!writeFile) {
		std::cerr << "\nWrite Error: Unable to write to file.\n\n";
		std::exit(EXIT_FAILURE);
	}
	
	// Write out to disk image file embedded ZIP file.
	writeFile.write((char*)&jdv.ImageVec[0], jdv.ImageVec.size());
	std::cout << "\nCreated output file: \"" + jdv.ZIP_NAME + " " << jdv.ImageVec.size() << " " << "Bytes\"\n"
		<< "You can now tweet this image.\n\n";
}

void displayInfo() {

	std::cout << R"(
twexif v1.0 for Twitter. Created by Nicholas Cleasby (@CleasbyCode) 14/07/2023.

Twitter will strip out metadata from a JPG image file when posted on its platform.

twexif is a simple command line tool that enables you to insert tiny files / metadata
(up to 10KB) into your JPG image file which Twitter will preserve.

For this program, and to also maximise the amount of data you can insert, 
you need to compress your file(s) into a ZIP archive.

Your ZIP file is stored within the ICC Profile of the JPG image file.

Usage example:

****
$ ./twexif  your_image.jpg  your_data_file.zip

Created output file: "twexif_img.jpg 199856 Bytes"
You can now tweet this image.
****

To get access to and extract your embedded data file(s) using Linux, 
just rename the .jpg file extension to .zip, and then from the Linux desktop,
click the file icon to open & extract the ZIP contents.

For Windows, first rename the .jpg file extension to .zip, 
then from a console type the following command:

****
PS C:\Demo> Expand-Archive  your_twexif_image.zip
****


)";
}
