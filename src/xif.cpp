// Xif v1.0 for Twitter. Created by Nicholas Cleasby (@CleasbyCode) 14/07/2023.
//
// Compile program (Linux): $ g++ xif.cpp -s -o xif
// Run program: ./xif

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

typedef unsigned char BYTE;
typedef unsigned short SBYTE;

struct xifStruct {
	std::vector<BYTE> ImageVec, ProfileVec;
	std::string Image_Name, Data_Name;
	size_t Image_Size{}, Data_Size{};
	const SBYTE DATA_FILE_LOCATION = 443;
};

// Update values, such as block size, file sizes and other values, and write them into the relevant vector index locations. Overwrites previous values.
class ValueUpdater {
public:
	void Value(std::vector<BYTE>& vec, size_t value_Insert_Index, const size_t VALUE, SBYTE bits, bool isBig) {
		if (isBig) { // (Big-endian)
			while (bits) vec[value_Insert_Index++] = (VALUE >> (bits -= 8)) & 0xff;
		}
		else { // For ZIP (Little-endian)
			while (bits) vec[value_Insert_Index--] = (VALUE >> (bits -= 8)) & 0xff;
		}
	}
} *update;

// Open user image & data file. Display error & exit program if any file fails to open.
void openFiles(xifStruct& xif);

// So that it remains a valid, working ZIP archive, adjust ZIP file offsets now that it's embedded in a different location within the image file. 
void fixZipOffset(xifStruct& xif);

// Write out to file the embedded image file.
void writeOutFile(xifStruct& xif);

// Display program infomation
void displayInfo();

int main(int argc, char** argv) {

	xifStruct xif;

	if (argc == 2 && std::string(argv[1]) == "--info") {
		displayInfo();
	}
	else if (argc > 2 && argc < 4) {
		xif.Image_Name = argv[1];
		xif.Data_Name = argv[2];
		openFiles(xif);
	}
	else {
		std::cout << "\nUsage:\txif  <jpg_image>  <data_file>\n\txif  --info\n\n";
	}
	return 0;
}

void openFiles(xifStruct& xif) {

	std::ifstream
		readImage(xif.Image_Name, std::ios::binary),
		readData(xif.Data_Name, std::ios::binary);

	if (!readImage || !readData) {
		// Open file failure, display relevant error message and exit program.

		const std::string READ_ERR_MSG = "\nRead Error: Unable to open/read file: ";
		const std::string ERR_MSG = !readImage ? READ_ERR_MSG + "\"" + xif.Image_Name + "\"\n\n" : READ_ERR_MSG + "\"" + xif.Data_Name + "\"\n\n";

		std::cerr << ERR_MSG;
		std::exit(EXIT_FAILURE);
	}

	// Get size of files.
	readImage.seekg(0, readImage.end),
	readData.seekg(0, readData.end);

	// Update file size variables.
	xif.Image_Size = readImage.tellg();
	xif.Data_Size = readData.tellg();

	// Reset read position of files. 
	readImage.seekg(0, readImage.beg),
	readData.seekg(0, readData.beg);

	// Data file size limit for JPG / Twitter is 10KB. We also have to take off 405 bytes for the basic iCC Profile, 
	// which leaves use just 9,835 bytes for the embedded file. Compressing your data file is recommended (ZIP/RAR, etc).
	const size_t MAX_FILE_SIZE = 9835;

	if (xif.Data_Size > MAX_FILE_SIZE) {
		// File size check failure, display error message and exit program.
		std::cerr << "\nFile Size Error:\n\n  Your data file size [" << xif.Data_Size << " Bytes] must not exceed 10KB.\n\n";
		std::exit(EXIT_FAILURE);
	}

	// The first 443 bytes of this vector contains the JPG header, iCC Profile header and the main (basic) iCC Profile.
	// Data file will be inserted & stored at the end of this vector.
	xif.ProfileVec = {
		0xFF, 0xD8, 0xFF, 0xE0, 0x00, 0x10, 0x4A, 0x46, 0x49, 0x46, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01,
		0x00, 0x01, 0x00, 0x00, 0xFF, 0xE2, 0x0E, 0x8C, 0x49, 0x43, 0x43, 0x5F, 0x50, 0x52, 0x4F, 0x46,
		0x49, 0x4C, 0x45, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x6c, 0x63, 0x6d, 0x73, 0x02, 0x10,
		0x00, 0x00, 0x6D, 0x6E, 0x74, 0x72, 0x52, 0x47, 0x42, 0x20, 0x58, 0x59, 0x5A, 0x20, 0x07, 0xE2,
		0x00, 0x03, 0x00, 0x14, 0x00, 0x09, 0x00, 0x0E, 0x00, 0x1D, 0x61, 0x63, 0x73, 0x70, 0x4D, 0x53,
		0x46, 0x54, 0x00, 0x00, 0x00, 0x00, 0x73, 0x61, 0x77, 0x73, 0x63, 0x74, 0x72, 0x6C, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF6, 0xD6, 0x00, 0x01,
		0x00, 0x00, 0x00, 0x00, 0xD3, 0x2D, 0x68, 0x61, 0x6E, 0x64, 0xEB, 0x77, 0x1F, 0x3C, 0xAA, 0x53,
		0x51, 0x02, 0xE9, 0x3E, 0x28, 0x6C, 0x91, 0x46, 0xAE, 0x57, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x64, 0x65, 0x73, 0x63, 0x00, 0x00,
		0x00, 0xF0, 0x00, 0x00, 0x00, 0x1C, 0x77, 0x74, 0x70, 0x74, 0x00, 0x00, 0x01, 0x0C, 0x00, 0x00,
		0x00, 0x14, 0x72, 0x58, 0x59, 0x5A, 0x00, 0x00, 0x01, 0x20, 0x00, 0x00, 0x00, 0x14, 0x67, 0x58,
		0x59, 0x5A, 0x00, 0x00, 0x01, 0x34, 0x00, 0x00, 0x00, 0x14, 0x62, 0x58, 0x59, 0x5A, 0x00, 0x00,
		0x01, 0x48, 0x00, 0x00, 0x00, 0x14, 0x72, 0x54, 0x52, 0x43, 0x00, 0x00, 0x01, 0x5C, 0x00, 0x00,
		0x00, 0x34, 0x67, 0x54, 0x52, 0x43, 0x00, 0x00, 0x01, 0x5C, 0x00, 0x00, 0x00, 0x34, 0x62, 0x54,
		0x52, 0x43, 0x00, 0x00, 0x01, 0x5C, 0x00, 0x00, 0x00, 0x34, 0x63, 0x70, 0x72, 0x74, 0x00, 0x00,
		0x01, 0x90, 0x00, 0x00, 0x00, 0x01, 0x64, 0x65, 0x73, 0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x05, 0x6E, 0x52, 0x47, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x58, 0x59, 0x5A, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF3, 0x54, 0x00, 0x01,
		0x00, 0x00, 0x00, 0x01, 0x16, 0xC9, 0x58, 0x59, 0x5A, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x6F, 0xA0, 0x00, 0x00, 0x38, 0xF2, 0x00, 0x00, 0x03, 0x8F, 0x58, 0x59, 0x5A, 0x20, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x62, 0x96, 0x00, 0x00, 0xB7, 0x89, 0x00, 0x00, 0x18, 0xDA, 0x58, 0x59,
		0x5A, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0xA0, 0x00, 0x00, 0x0F, 0x85, 0x00, 0x00,
		0xB6, 0xC4, 0x63, 0x75, 0x72, 0x76, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00,
		0x01, 0x07, 0x02, 0xB5, 0x05, 0x6B, 0x09, 0x36, 0x0E, 0x50, 0x14, 0xB1, 0x1C, 0x80, 0x25, 0xC8,
		0x30, 0xA1, 0x3D, 0x19, 0x4B, 0x40, 0x5B, 0x27, 0x6C, 0xDB, 0x80, 0x6B, 0x95, 0xE3, 0xAD, 0x50,
		0xC6, 0xC2, 0xE2, 0x31, 0xFF, 0xFF, 0x20, 0x78, 0x69, 0x66, 0x20
	};

	// Read-in and store JPG image file into vector "ImageVec".
	xif.ImageVec.resize(xif.Image_Size / sizeof(BYTE));
	readImage.read((char*)xif.ImageVec.data(), xif.Image_Size);

	// Read-in and store user's data file into vector "ProfileVec".
	xif.ProfileVec.resize(xif.Data_Size + xif.ProfileVec.size() / sizeof(BYTE));
	readData.read((char*)&xif.ProfileVec[443], xif.Data_Size);

	const std::string
		JPG_SIG = "\xFF\xD8\xFF",	// JPG image signature. 
		ZIP_SIG = "PK\x03\x04",		// ZIP file signature
		JPG_CHECK{ xif.ImageVec.begin(), xif.ImageVec.begin() + JPG_SIG.length() },															                            // Get JPG image header from vector. 
		ZIP_CHECK{ xif.ProfileVec.begin() + xif.DATA_FILE_LOCATION, xif.ProfileVec.begin() + xif.DATA_FILE_LOCATION + ZIP_SIG.length() };	// Get ZIP header from vector.

	// Make sure we are dealing with valid JPG image file.
	if (JPG_CHECK != JPG_SIG) {

		// File requirements check failure, display relevant error message and exit program.
		std::cerr << "\nImage Format Error: File does not appear to be a valid JPG image file.\n\n";
		std::exit(EXIT_FAILURE);
	}

	const std::string // Header IDs
		EXIF_SIG = "Exif\x00\x00II",
		EXIF_END_SIG = "xpacket end",
		ICC_PROFILE_SIG = "ICC_PROFILE";

	// An embedded jpg thumbnail will cause problems with this program. So search and remove blocks that may contain a jpg thumbnail.

	// Check for a ICC_PROFILE and delete all content before the beginning of the Profile. This removes any embedded jpg thumbnail before Profile.
	// Profile will be deleted later.
	const size_t ICC_PROFILE_POS = search(xif.ImageVec.begin(), xif.ImageVec.end(), ICC_PROFILE_SIG.begin(), ICC_PROFILE_SIG.end()) - xif.ImageVec.begin();
	if (xif.ImageVec.size() > ICC_PROFILE_POS) {
		xif.ImageVec.erase(xif.ImageVec.begin(), xif.ImageVec.begin() + ICC_PROFILE_POS);
	}

	// If no Profile found, search for Exif block (look for end signature "xpacket end") and remove the block.
	const size_t EXIF_END_POS = search(xif.ImageVec.begin(), xif.ImageVec.end(), EXIF_END_SIG.begin(), EXIF_END_SIG.end()) - xif.ImageVec.begin();
	if (xif.ImageVec.size() > EXIF_END_POS) {
		xif.ImageVec.erase(xif.ImageVec.begin(), xif.ImageVec.begin() + (EXIF_END_POS + 17));
	}

	// Remove Exif block that has no "xpacket end" signature.
	const size_t EXIF_START_POS = search(xif.ImageVec.begin(), xif.ImageVec.end(), EXIF_SIG.begin(), EXIF_SIG.end()) - xif.ImageVec.begin();
	if (xif.ImageVec.size() > EXIF_START_POS) {
		// get size of Exif block
		const size_t EXIF_BLOCK_SIZE = xif.ImageVec[EXIF_START_POS - 2] << 8 | xif.ImageVec[EXIF_START_POS - 1];
		xif.ImageVec.erase(xif.ImageVec.begin(), xif.ImageVec.begin() + EXIF_BLOCK_SIZE - 2);
	}

	// ^ Any jpg embedded thumbnail should have now been removed...

	// Signature for Define Quantization Table(s) 
	const auto DQT_SIG = { 0xFF, 0xDB };

	// Find location in vector "ImageVec" of first DQT index location of the image file.
	const size_t DQT_POS = search(xif.ImageVec.begin(), xif.ImageVec.end(), DQT_SIG.begin(), DQT_SIG.end()) - xif.ImageVec.begin();

	// Erase the first n bytes of the JPG header before this DQT position. We later replace the erased header with the contents of vector "ProfileVec".
	xif.ImageVec.erase(xif.ImageVec.begin(), xif.ImageVec.begin() + DQT_POS);

	SBYTE
		Bits = 16,				// Variable used with the "updateValue" function.
		Profile_Header_Size_Field_Index = 22,	// "ProfileVec" first index location for the 2 byte iCC Profile (header) length field, of the jpg image.
		Profile_Main_Size_Field_Index = 38;	// "ProfileVec" second index location for the 4 byte iCC Profile (main) length field, of the jpg image (only 2 bytes used).

	const size_t
		VECTOR_SIZE = xif.ProfileVec.size() - Profile_Header_Size_Field_Index;	// Get updated size for vector "ProfileVec" after adding user's data file.

	// Update size of the first length field of the iCC Profile.
	update->Value(xif.ProfileVec, Profile_Header_Size_Field_Index, VECTOR_SIZE, Bits, true);

	Bits += 16; // 32

	// Update size of the second (main) length field of the iCC Profile (always 16 bytes less than the first).
	update->Value(xif.ProfileVec, Profile_Main_Size_Field_Index, VECTOR_SIZE - 16, Bits, true);

	// If file is ZIP, adjust ZIP file offsets as their location has now changed.
	if (ZIP_CHECK == ZIP_SIG) {
		fixZipOffset(xif);
	}

	// Insert contents of vector "ProfileVec" into vector "ImageVec", combining the iCC Profile and user's data file with the JPG image.	
	xif.ImageVec.insert(xif.ImageVec.begin(), xif.ProfileVec.begin(), xif.ProfileVec.end());

	xif.Data_Name = "xif_img.jpg";

	writeOutFile(xif);
}

void fixZipOffset(xifStruct& xif) {

	const std::string	// Header IDs
		START_CENTRAL_SIG = "PK\x01\x02",
		END_CENTRAL_SIG = "PK\x05\x06",
		ZIP_SIG = "PK\x03\x04";

	// Search vector to get index locations for "Start Central Directory" & "End Central Directory".
	const size_t
		START_CENTRAL_INDEX = search(xif.ProfileVec.begin() + xif.DATA_FILE_LOCATION, xif.ProfileVec.end(), START_CENTRAL_SIG.begin(), START_CENTRAL_SIG.end()) - xif.ProfileVec.begin(),
		END_CENTRAL_INDEX = search(xif.ProfileVec.begin() + START_CENTRAL_INDEX, xif.ProfileVec.end(), END_CENTRAL_SIG.begin(), END_CENTRAL_SIG.end()) - xif.ProfileVec.begin();

	size_t
		Zip_Records_Index = END_CENTRAL_INDEX + 11,		// Index location for ZIP file records value.
		End_Central_Start_Index = END_CENTRAL_INDEX + 19,	// Index location for End Central Start offset.
		Central_Local_Index = START_CENTRAL_INDEX - 1,		// Initialise variable to just before Start Central index location.
		New_Offset = xif.DATA_FILE_LOCATION - 1,		// Initialise variable to near location of ZIP file.
		Zip_Records = (xif.ProfileVec[Zip_Records_Index] << 8) | xif.ProfileVec[Zip_Records_Index - 1];	// Get ZIP file records value from index location within vector.

	// Starting from the last IDAT chunk, search for ZIP file record offsets and update them to their new offset location.
	while (Zip_Records--) {
		New_Offset = search(xif.ProfileVec.begin() + New_Offset + 1, xif.ProfileVec.end(), ZIP_SIG.begin(), ZIP_SIG.end()) - xif.ProfileVec.begin(),
		Central_Local_Index = 45 + search(xif.ProfileVec.begin() + Central_Local_Index, xif.ProfileVec.end(), START_CENTRAL_SIG.begin(), START_CENTRAL_SIG.end()) - xif.ProfileVec.begin();
		update->Value(xif.ProfileVec, Central_Local_Index, New_Offset, 32, false);
	}

	// Write updated "Start Central Directory" offset into End Central Directory's "Start Central Directory" index location within vector.
	update->Value(xif.ProfileVec, End_Central_Start_Index, START_CENTRAL_INDEX, 32, false);
}

void writeOutFile(xifStruct& xif) {

	std::ofstream writeFile(xif.Data_Name, std::ios::binary);

	if (!writeFile) {
		std::cerr << "\nWrite Error: Unable to write to file.\n\n";
		std::exit(EXIT_FAILURE);
	}

	// Write out to disk image file embedded with the encrypted data file.
	writeFile.write((char*)&xif.ImageVec[0], xif.ImageVec.size());
	std::cout << "\nCreated output file: \"" + xif.Data_Name + " " << xif.ImageVec.size() << " " << "Bytes\"\n"
		<< "You can now tweet this image file.\n\n";

}

void displayInfo() {

	std::cout << R"(
Xif v1.0 for Twitter. Created by Nicholas Cleasby (@CleasbyCode) 14/07/2023.

Twitter will strip out metadata from a JPG image file when posted on its platform.

Xif is a command line tool that enables you to embed tiny files 
(up to 10KB) into your JPG image which Twitter will preserve, so your data file travels with the image.

To maximise the amount of data you can embed, it is recommended to compress (ZIP/RAR, etc) your file.

The data file is stored within the iCC Profile of the JPG image file.

Usage examples (Linux):

$ ./xif  your_image.jpg  your_data_file.zip (or your_data_file.rar)

Created output file: "xif_img.jpg 9156 Bytes"
You can now tweet this image file.
---

To get access to and extract your embedded data file for a ZIP or RAR embedded file under Linux desktop, just rename 
the .jpg file extension to .zip, then click the file icon to open & extract the ZIP/RAR contents.

For a RAR file under Linux, from a Linux terminal, you can just enter the command: $ unrar e xif_img.jpg

For a embedded ZIP file under Windows. Rename the .jpg file extension to .zip. From a command console type the following command:

PS C:\Demo> Expand-Archive .\xif_image.zip

For a RAR file under Windows, you will need to use a program such as WinRar to extract your file from the image.


)";
}
