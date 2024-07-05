// ZIP file has been moved to another location. We need to find and adjust the ZIP file record offsets to their new location.
void adjustZipOffsets(std::vector<uint_fast8_t>& Vec) {
 
	constexpr uint_fast8_t 
		ZIP_SIG[] 			{0x50, 0x4B, 0x03, 0x04},
		START_CENTRAL_DIR_SIG[] 	{0x50, 0x4B, 0x01, 0x02},
		END_CENTRAL_DIR_SIG[] 		{0x50, 0x4B, 0x05, 0x06};

	const uint_fast32_t
		START_CENTRAL_DIR_INDEX = searchFunc(Vec, 0, 0, START_CENTRAL_DIR_SIG),
		END_CENTRAL_DIR_INDEX = searchFunc(Vec, START_CENTRAL_DIR_INDEX, 0, END_CENTRAL_DIR_SIG);

	uint_fast32_t
		zip_records_index = END_CENTRAL_DIR_INDEX + 0x0B,
		end_central_start_index = END_CENTRAL_DIR_INDEX + 0x13,
		central_local_index = START_CENTRAL_DIR_INDEX - 1,
		new_offset{};

	uint_fast16_t 
		zip_records = (static_cast<uint_fast16_t>(Vec[zip_records_index]) << 8) | 
				static_cast<uint_fast16_t>(Vec[zip_records_index - 1]);

	uint_fast8_t bits = 32;

	while (zip_records--) {
		new_offset = searchFunc(Vec, new_offset, 1, ZIP_SIG),
		central_local_index = 0x2D + searchFunc(Vec, central_local_index, 0, START_CENTRAL_DIR_SIG);
		valueUpdater(Vec, central_local_index, new_offset, bits, false);
	}

	valueUpdater(Vec, end_central_start_index, START_CENTRAL_DIR_INDEX, bits, false);
}
