// Xif v1.1 for Twitter. Created by Nicholas Cleasby (@CleasbyCode) 14/07/2023.
//
// Compile program (Linux): $ g++ xif.cpp -O2 -s -o xif
// Run program: ./xif

#include <algorithm>
#include <fstream>
#include <iostream>
#include <cstdint>
#include <string>
#include <vector>

typedef unsigned char BYTE;

// Update values, such as block size, file sizes and other values and write them into the relevant vector index locations. Overwrites previous values.
class Value_Updater {
public:
	void Value(std::vector<BYTE>& vec, size_t value_insert_index, const size_t VALUE, int bits, bool isBig) {
		if (isBig) { // (Big-endian format)
			while (bits) vec[value_insert_index++] = (VALUE >> (bits -= 8)) & 0xff;
		}
		else { // For ZIP (Little-endian format)
			while (bits) vec[value_insert_index--] = (VALUE >> (bits -= 8)) & 0xff;
		}
	}
} *update;

// Open user image & data file & proceed to embed user's data file & write out to disk embedded image. 
void Embed_File(const std::string&, const std::string&);

// Display program information
void Display_Info();

int main(int argc, char** argv) {

	if (argc == 2 && std::string(argv[1]) == "--info") {
		Display_Info();
	}
	else if (argc > 2 && argc < 4) {
		const std::string 
			IMAGE_NAME = argv[1],
			DATA_NAME = argv[2];
		
		Embed_File(IMAGE_NAME, DATA_NAME);
	}
	else {
		std::cout << "\nUsage: xif <jpg_image> <data_file>\n\t\bxif --info\n\n";
	}
	return 0;
}

void Embed_File(const std::string& IMAGE_NAME, const std::string& DATA_NAME) {

	std::ifstream
		read_image_fs(IMAGE_NAME, std::ios::binary),
		read_data_fs(DATA_NAME, std::ios::binary);

	if (!read_image_fs || !read_data_fs) {
		// Open file failure, display relevant error message and exit program.

		const std::string READ_ERR_MSG = "\nRead Error: Unable to open/read file: ";
		const std::string ERR_MSG = !read_image_fs ? READ_ERR_MSG + "\"" + IMAGE_NAME + "\"\n\n" : READ_ERR_MSG + "\"" + DATA_NAME + "\"\n\n";

		std::cerr << ERR_MSG;

		std::exit(EXIT_FAILURE);
	}

	// The first 443 bytes of this vector contains the JPG header, iCC Profile header and the main iCC Profile.
	// Data file will be inserted & stored at the end of this vector.
	std::vector<BYTE>Profile_Vec = {
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
	},

	// Read-in and store JPG image file into vector "Image_Vec".
	Image_Vec((std::istreambuf_iterator<char>(read_image_fs)), std::istreambuf_iterator<char>());

	// Read-in and store user's data file into vector "Profile_Vec" at the end of the basic profile.
	Profile_Vec.insert(Profile_Vec.end(), std::istreambuf_iterator<char>(read_data_fs), std::istreambuf_iterator<char>());

	// Data file size limit for Twitter is 10KB. We also have to take off 443 bytes for the basic iCC Profile, 
	// which leaves just 9,797 bytes for the embedded file. 
	// To maximise the amount of embedded data, compressing your data file is recommended (ZIP/RAR, etc).
	const size_t MAX_FILE_SIZE = 9797;

	if (Profile_Vec.size() > MAX_FILE_SIZE) {
		// File size check failure, display error message and exit program.
		std::cerr << "\nFile Size Error: Your data file size must not exceed 10KB.\n\n";
		std::exit(EXIT_FAILURE);
	}

	const int DATA_FILE_LOCATION = 443;

	const std::string
		JPG_SIG = "\xFF\xD8\xFF",	// JPG image signature. 
		ZIP_SIG = "PK\x03\x04",		// ZIP file signature
		JPG_CHECK{ Image_Vec.begin(), Image_Vec.begin() + JPG_SIG.length() },													// Get JPG image header from vector. 
		ZIP_CHECK{ Profile_Vec.begin() + DATA_FILE_LOCATION, Profile_Vec.begin() + DATA_FILE_LOCATION + ZIP_SIG.length() };	// Get ZIP header from vector.

	// Make sure we are dealing with valid JPG image file.
	if (JPG_CHECK != JPG_SIG) {

		// File requirements check failure, display relevant error message and exit program.
		std::cerr << "\nImage Format Error: File does not appear to be a valid JPG image file.\n\n";
		std::exit(EXIT_FAILURE);
	}

	const std::string
		EXIF_SIG = "Exif\x00\x00II",
		EXIF_END_SIG = "xpacket end",
		ICC_PROFILE_SIG = "ICC_PROFILE";

	// An embedded JPG thumbnail will cause problems with this program. So search and remove blocks that may contain a thumbnail image.

	// Check for a iCC Profile and delete all content before the beginning of the profile. This removes any embedded thumbnail image before profile.
	// The actual profile will be deleted later.
	const size_t ICC_PROFILE_POS = search(Image_Vec.begin(), Image_Vec.end(), ICC_PROFILE_SIG.begin(), ICC_PROFILE_SIG.end()) - Image_Vec.begin();
	if (Image_Vec.size() > ICC_PROFILE_POS) {
		Image_Vec.erase(Image_Vec.begin(), Image_Vec.begin() + ICC_PROFILE_POS);
	}

	// If no profile found, search for and Exif block (look for end signature "xpacket end") and remove the block.
	const size_t EXIF_END_POS = search(Image_Vec.begin(), Image_Vec.end(), EXIF_END_SIG.begin(), EXIF_END_SIG.end()) - Image_Vec.begin();
	if (Image_Vec.size() > EXIF_END_POS) {
		Image_Vec.erase(Image_Vec.begin(), Image_Vec.begin() + (EXIF_END_POS + 17));
	}

	// Remove and Exif block that has no "xpacket end" signature.
	const size_t EXIF_START_POS = search(Image_Vec.begin(), Image_Vec.end(), EXIF_SIG.begin(), EXIF_SIG.end()) - Image_Vec.begin();
	if (Image_Vec.size() > EXIF_START_POS) {
		// get size of Exif block
		const size_t EXIF_BLOCK_SIZE = Image_Vec[EXIF_START_POS - 2] << 8 | Image_Vec[EXIF_START_POS - 1];
		Image_Vec.erase(Image_Vec.begin(), Image_Vec.begin() + EXIF_BLOCK_SIZE - 2);
	}

	// ^ Any JPG embedded thumbnail should have now been removed.

	// Signature for Define Quantization Table(s) 
	const auto DQT_SIG = { 0xFF, 0xDB };

	// Find location in vector "Image_Vec" of first DQT index location of the image file.
	const size_t DQT_POS = search(Image_Vec.begin(), Image_Vec.end(), DQT_SIG.begin(), DQT_SIG.end()) - Image_Vec.begin();

	// Erase the first n bytes of the JPG header before this DQT position. We will replace the erased header with the contents of vector "Profile_Vec".
	Image_Vec.erase(Image_Vec.begin(), Image_Vec.begin() + DQT_POS);

	// JPG Signature for Start-Of-Frame Progressive (SOFP) block. 
	// Images saved/downloaded from Twitter always have this SOFP sig. 
	// Twitter may not always preserve embedded data posted with JPG images not encodded with a SOFP block. 
	// Always best to use a Twitter encoded JPG image.
	const auto SOFP_SIG = { 0xFF, 0xC2 };

	// Find location in vector "Image_Vec" of the SOFP index location  within the image file.
	const size_t SOFP_POS = search(Image_Vec.begin(), Image_Vec.end(), SOFP_SIG.begin(), SOFP_SIG.end()) - Image_Vec.begin();

	if (SOFP_POS == Image_Vec.size()) {
		std::cerr << "\nImage Error: Image does not appear to be a Twitter encoded JPG file.\n\n"
			"For compatibility reasons, please use a JPG image from Twitter.\n\nIf you still want to use this image with Xif, first post it to Twitter,\nclick the image to fully expand it, save image, then try again.\n\n";

		std::exit(EXIT_FAILURE);
	}

	uint8_t
		bits = 16,				// Variable used with the "Update_Value" function.
		profile_header_size_index = 22,		// "Profile_Vec" first index location for the 2 byte JPG iCC Profile header length field.
		profile_main_size_index = 38;		// "Profile_Vec" second index location for the 4 byte main iCC Profile length field (only 2 bytes used).

	const size_t VECTOR_SIZE = Profile_Vec.size() - profile_header_size_index;	// Get updated size for vector "Profile_Vec" after adding user's data file.

	// Update size of the first length field of the JPG iCC Profile header.
	update->Value(Profile_Vec, profile_header_size_index, VECTOR_SIZE, bits, true);

	bits += 16; // 32

	// Update size of the second length field of the main iCC Profile (length always 16 bytes less than the first).
	update->Value(Profile_Vec, profile_main_size_index, VECTOR_SIZE - 16, bits, true);

	// If file is ZIP, adjust ZIP file offsets as their location has now changed.
	if (ZIP_CHECK == ZIP_SIG) {
		const std::string	// Header ID's
			START_CENTRAL_DIR_SIG = "PK\x01\x02",
			END_CENTRAL_DIR_SIG = "PK\x05\x06",
			ZIP_SIG = "PK\x03\x04";

		// Search vector to get index locations for the "Start Central Directory" & "End Central Directory".
		const size_t
			START_CENTRAL_DIR_INDEX = search(Profile_Vec.begin() + DATA_FILE_LOCATION, Profile_Vec.end(), START_CENTRAL_DIR_SIG.begin(), START_CENTRAL_DIR_SIG.end()) - Profile_Vec.begin(),
			END_CENTRAL_DIR_INDEX = search(Profile_Vec.begin() + START_CENTRAL_DIR_INDEX, Profile_Vec.end(), END_CENTRAL_DIR_SIG.begin(), END_CENTRAL_DIR_SIG.end()) - Profile_Vec.begin();

		size_t
			zip_records_index = END_CENTRAL_DIR_INDEX + 11,		// Index location for ZIP file records value.
			end_central_start_index = END_CENTRAL_DIR_INDEX + 19,	// Index location for End Central Start offset.
			central_local_index = START_CENTRAL_DIR_INDEX - 1,	// Initialise variable to just before Start Central index location.
			new_offset = DATA_FILE_LOCATION - 1,			// Initialise variable to near location of ZIP file.
			zip_records = (Profile_Vec[zip_records_index] << 8) | Profile_Vec[zip_records_index - 1];	// Get ZIP file records value from index location within vector.

		// Starting from the last IDAT chunk, search for ZIP file record offsets and update them to their new offset location.
		while (zip_records--) {
			new_offset = search(Profile_Vec.begin() + new_offset + 1, Profile_Vec.end(), ZIP_SIG.begin(), ZIP_SIG.end()) - Profile_Vec.begin(),
			central_local_index = 45 + search(Profile_Vec.begin() + central_local_index, Profile_Vec.end(), START_CENTRAL_DIR_SIG.begin(), START_CENTRAL_DIR_SIG.end()) - Profile_Vec.begin();
			update->Value(Profile_Vec, central_local_index, new_offset, 32, false);
		}

		// Write updated "Start Central Directory" offset into End Central Directory's "Start Central Directory" index location within vector.
		update->Value(Profile_Vec, end_central_start_index, START_CENTRAL_DIR_INDEX, 32, false);
	}

	// Insert contents of vector "Profile_Vec" into vector "Image_Vec", combining the iCC Profile and user's data file with the JPG image.	
	Image_Vec.insert(Image_Vec.begin(), Profile_Vec.begin(), Profile_Vec.end());

	const std::string EMBEDDED_IMAGE_NAME = "xif_img.jpg";

	std::ofstream write_file_fs(EMBEDDED_IMAGE_NAME, std::ios::binary);

	if (!write_file_fs) {
		std::cerr << "\nWrite Error: Unable to write to file.\n\n";
		std::exit(EXIT_FAILURE);
	}

	// Write out to disk image file embeddedd with user's data file.
	write_file_fs.write((char*)&Image_Vec[0], Image_Vec.size());
	std::cout << "\nCreated output file: \"" + EMBEDDED_IMAGE_NAME + " " << Image_Vec.size() << " " << "Bytes\"\n"
		<< "You can now post this data-embedded image file to Twitter.\n\n";
}

void Display_Info() {

	std::cout << R"(
Xif v1.1 for Twitter. Created by Nicholas Cleasby (@CleasbyCode) 14/07/2023.

https://github.com/CleasbyCode/xif

)";
}
