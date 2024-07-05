void startJzp(const std::string& IMAGE_FILENAME, const std::string& DATA_FILENAME) {

	constexpr uint_fast16_t MAX_FILE_SIZE = 9797;
	constexpr uint_fast8_t MIN_IMAGE_SIZE = 134;

	const size_t 
		TMP_DATA_FILE_SIZE = std::filesystem::file_size(DATA_FILENAME),
		TMP_IMAGE_FILE_SIZE = std::filesystem::file_size(IMAGE_FILENAME);

	if (TMP_DATA_FILE_SIZE > MAX_FILE_SIZE || MIN_IMAGE_SIZE > TMP_IMAGE_FILE_SIZE) {		
		std::cerr << "\nFile Size Error: " 
			<< (MIN_IMAGE_SIZE > TMP_IMAGE_FILE_SIZE
        			? "Image is too small to be a valid JPG image"
	        		: "Data file exceeds the maximum limit of " + std::to_string(MAX_FILE_SIZE)
			+ ".\n\n");

    		std::exit(EXIT_FAILURE);
	}
	
	std::ifstream
		image_file_ifs(IMAGE_FILENAME, std::ios::binary),
		data_file_ifs(DATA_FILENAME, std::ios::binary);

	if (!image_file_ifs || !data_file_ifs) {
		std::cerr << "\nRead File Error: Unable to read " << (!image_file_ifs 
			? "image file" 
			: "data file") 
		<< ".\n\n";

		std::exit(EXIT_FAILURE);
	}
	
	std::vector<uint_fast8_t>Image_Vec((std::istreambuf_iterator<char>(image_file_ifs)), std::istreambuf_iterator<char>());
	
	constexpr uint_fast8_t
		SOI_SIG[] 	{ 0xFF, 0xD8 },
		EOI_SIG[] 	{ 0xFF, 0xD9 },
		ZIP_SIG[] 	{ 0x50, 0x4B, 0x03, 0x04 };
	
	if (!std::equal(std::begin(SOI_SIG), std::end(SOI_SIG), std::begin(Image_Vec)) 
		|| !std::equal(std::begin(EOI_SIG), std::end(EOI_SIG), std::end(Image_Vec) - 2)) {
        		std::cerr << "\nImage File Error: This is not a valid JPG image.\n\n";
			std::exit(EXIT_FAILURE);
    	}
		
	Profile_Vec.insert(Profile_Vec.end(), std::istreambuf_iterator<char>(data_file_ifs), std::istreambuf_iterator<char>());
	
	eraseSegments(Image_Vec);
	
	uint_fast8_t
		bits = 16,				
		profile_header_size_index = 22,		
		profile_main_size_index = 38;	

	const uint_fast32_t VECTOR_SIZE = static_cast<uint_fast32_t>(Profile_Vec.size() - profile_header_size_index);	

	valueUpdater(Profile_Vec, profile_header_size_index, VECTOR_SIZE, bits, true);

	bits = 32; 

	valueUpdater(Profile_Vec, profile_main_size_index, VECTOR_SIZE - 16, bits, true);

	const uint_fast32_t DATA_FILE_INDEX = 443;

	if (std::equal(std::begin(ZIP_SIG), std::end(ZIP_SIG), std::begin(Profile_Vec) + DATA_FILE_INDEX))  {
        	adjustZipOffsets(Profile_Vec);
    	}
				 
	Image_Vec.insert(Image_Vec.begin(), Profile_Vec.begin(), Profile_Vec.end());
	
	const uint_fast32_t EMBEDDED_IMAGE_SIZE = static_cast<uint_fast32_t>(Image_Vec.size());
				   
	const std::string EMBEDDED_IMAGE_NAME = "jzp_img.jpg";

	std::ofstream file_ofs(EMBEDDED_IMAGE_NAME, std::ios::binary);

	if (!file_ofs) {
		std::cerr << "\nWrite Error: Unable to write to file.\n\n";
		std::exit(EXIT_FAILURE);
	}

	file_ofs.write((char*)&Image_Vec[0], EMBEDDED_IMAGE_SIZE);
	std::cout << "\nCreated output file: \"" + EMBEDDED_IMAGE_NAME + " " << EMBEDDED_IMAGE_SIZE << " Bytes\"\n\n"
		<< "You can now post this data-embedded image file to X/Twitter.\n\n";
}
