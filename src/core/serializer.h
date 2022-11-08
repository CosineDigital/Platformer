#ifndef SERIALIZER_H_
#define SERIALIZER_H_

#include <cstring>
#include <fstream>

namespace serializer {

	// headers used by the 
	namespace detail {

		// file meta data and constants
		static char FILE_IDENTITY__[4] = "LVL";
		static uint32_t FILE_IDENTITY = *(uint32_t*)&FILE_IDENTITY__;
		static constexpr uint32_t FILE_VERSION = 1UL;

		// disable padding
		#pragma pack(1)

		struct FileHeader {
			uint32_t identity; // 4 bytes for the identity
			uint32_t version;
		};

		static_assert(sizeof(FileHeader) == 8);


		// The tile data header : needed for knowing how many tiles are stored the file
		struct TileDataHeader {
			// Just the width and length of the level
			uint32_t width; // 4 bytes
			uint32_t height; // 4 bytes
		};

		static_assert(sizeof(TileDataHeader) == 8);

		struct TileEntityDataHeader {
			uint32_t count;
		};

		static_assert(sizeof(TileEntityDataHeader) == 4);


		// The entity data header : needed for knowing how many entities are stored in the file
		struct EntityDataHeader {
			uint32_t count; // 4 bytes for the number of entities stored
		};

		static_assert(sizeof(EntityDataHeader) == 4);

		// enable padding again
		#pragma pack()
	}

	/**
	* Load a level from a file
	* @param intoScene - The level to be loaded into
	* @param fromFile - The binary file to read from. Should end with ".lvl"
	*/
	static int loadLevel(Level* intoLevel, std::string fromFile)
	{
		std::ifstream in(fromFile, std::ios::binary);

		if (!in.is_open() | in.bad()) {
			std::cerr << "Error while loading level from file [ " << fromFile << " ]\n";
			return -1;
		}

		detail::FileHeader fileHeader;
		in.read((char*)&fileHeader, sizeof(detail::FileHeader));

		if (fileHeader.identity == detail::FILE_IDENTITY) {
			
			// Read the tile data into the level
			detail::TileDataHeader tileDataHeader; // create and read
			in.read((char*)&tileDataHeader, sizeof(detail::TileDataHeader));

			intoLevel->width = tileDataHeader.width;
			intoLevel->height = tileDataHeader.height;
			//intoLevel->allocateForTileData();

			in.read((char*)&intoLevel->tileData[0], sizeof(uint32_t) * tileDataHeader.width * tileDataHeader.height);

			// Read the entity data into the level
			detail::EntityDataHeader entityDataHeader; // create and read
			in.read((char*)&entityDataHeader, sizeof(detail::EntityDataHeader));

			intoLevel->entityCount = entityDataHeader.count;
			//intoLevel->allocateForEntityData();

			//in.read((char*)&intoLevel->entityData[0], sizeof(Entity) * entityDataHeader.count);

			in.close();

			return 0;
		}
		else
		{
			std::cerr << "Invalid data file " << fromFile << std::endl;
			return -1;
		}
	}

	/**
	* Write the scene into the file at the path at the
	* @param fromScene - The scene to save to file
	* @param intoFile - The binary file to save to. Ends with ".lvl"
	*/
	static int saveLevel(Level* fromLevel, std::string intoFile)
	{
		std::ofstream out(intoFile, std::ios::binary);

		if (!out.is_open() || out.bad()) {
			std::cerr << "Error while saving level into file [ " << intoFile << " ]\n";
			return -1;
		}

		detail::FileHeader fileHeader;
	
		fileHeader.identity = detail::FILE_IDENTITY;
		fileHeader.version = detail::FILE_VERSION;

		out.write((char*)&fileHeader, sizeof(detail::FileHeader)); 
		out.flush();


		// Write all tiles
		// Each tile should be an unsigned integer
		out.write((char*)&fromLevel->tileData[0], sizeof(uint32_t) * fromLevel->width * fromLevel->height);
		out.flush();
		
		// Write all tile entities
		detail::TileEntityDataHeader tileEntityDataHeader;
		tileEntityDataHeader.count = fromLevel->tileEntityCount;

		//out.write((char*)&tileEntityDataHeader, );

		// Write all entities

		
		// Create and write the entity data header to file and flush
		detail::EntityDataHeader entityDataHeader;
		entityDataHeader.count = fromLevel->entityCount;

		out.write((char*)&entityDataHeader, sizeof(detail::EntityDataHeader));
		out.flush();

		// Write the entities to file and flush
		//out.write((char*)&fromLevel->entityData[0], sizeof(Entity) * fromLevel->entityCount);
		out.flush();

		// close the file here
		out.close();

		return 0;
	}



	namespace detail {

		static void writeTiles(std::ofstream* out, Level* fromLevel) noexcept {

			TileDataHeader tileDataHeader {};
			tileDataHeader.width = fromLevel->width;
			tileDataHeader.height = fromLevel->height;

			out->write((char*)&tileDataHeader, sizeof(detail::TileDataHeader));
			out->flush();
			
			for (int i = 0; i < fromLevel->width; i++) { // x
				for (int j = 0; j < fromLevel->width; j++) { // y
					//out->write((char*)&fromLevel->getTile(i, j), sizeof(uint32_t));
				}
				out->flush(); // flush after every vertical layer
			}


		}
	}
}

#endif // !SERIALIZER_H_

