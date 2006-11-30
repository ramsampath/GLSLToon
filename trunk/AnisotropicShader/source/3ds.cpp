/*
**************************************************************************
Author: Terry Caton
Date: January 24th, 2001
Contact: tcaton@umr.edu
Open Source. Use as you wish, but don't bitch if it breaks something. If you
use this in any project worth mentioning, let me know, I'd love to see what 
you've done. 	
	  
Here's the framework for my 3D Studio File Loader. Currently, it supports
loading the vertex/index list (of course), along with materials and hierarchy
information. This is only a framework, however; you need to supply the code
to input the data into your own buffers, as each API varies widely, as does
each persons preference of how to organize data. At each point where data is
read from the file, take that data and feed it into a buffer, or just discard
it if you wish. 

The way I did it, and the way the framework here encourages, is to save all
the data into a 3dsObject class (one for each object in the file), and then at
the end when everything is done, assemble each into it's compiled vertex array/
vertex buffer/whatever. Granted, this is my first attempt at a 3D engine, so 
there may be a few fundamental flaws here, but hey, go easy on me.

If you so desire, contact me at tcaton@umr.edu. Questions, comments, money, 
whatever. Source to my engine is also available upon request, with a working 
example of this file loader in action. Thanks, and enjoy!!


Special thanks to John Ratcliff and all the guys I worked with, for getting
me interested in all this. 


**************************************************************************
*/



#include "3ds.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "CMesh.h"


// Global instance of loader
Load3ds gLoad3ds;

Load3ds::Load3ds()
{
	mBuffer = 0;
	mCurrentChunk = 0;
	mTempChunk = new Chunk;

	// initialize any other members you choose to add
	mModelData = new ModelData;
	// create the mesh that will store the 3d model information
	mMesh = new CMesh;
}


Load3ds::~Load3ds()
{
	delete mTempChunk;
	delete mModelData;
}


CMesh* Load3ds::Create(char * aFilename)
{
	int lBytesRead = 0;
	mCurrentChunk = new Chunk;

	mFile = fopen(aFilename, "rb");
	ReadChunk(mCurrentChunk);
	if (mCurrentChunk->mID != PRIMARY)
		exit(1107);
	// Largest possible buffer we'd need is MAX_INDICES * sizeof(triangle)
	mBuffer = new unsigned char[65536 * 3 * sizeof(float)];

	// Begin loading objects, by calling recursive function
	ProcessNextChunk(mCurrentChunk);

	// All data read, compile them into a friendly format
	CompileObjects();

	// Clean up after everything, to prepare for next time
	CleanUp();

	return this->mMesh;
}


int Load3ds::CleanUp()
{

	fclose(mFile);
	delete [] mBuffer;
	delete mCurrentChunk;
	
/*
Add any cleanup routines you may need between calls to Load3ds::Create(),
such as deleting temporary vertex/index lists, materials, or whatever
else you choose to add
*/

	return 1;
}


int Load3ds::ProcessNextChunk(Chunk * aPreviousChunk)
{
	mCurrentChunk = new Chunk;

	while (aPreviousChunk->mBytesRead < aPreviousChunk->mLength)
	{
		// Read next chunk
		ReadChunk(mCurrentChunk);

		switch (mCurrentChunk->mID)
		{
		case VERSION:
			// Check version (must be 3 or less)
			mCurrentChunk->mBytesRead += fread(mBuffer, 1, mCurrentChunk->mLength - mCurrentChunk->mBytesRead, mFile);
			if (*(unsigned short *) mBuffer > 0x03)
				exit(1107);
			break;

		case EDITMATERIAL:
			// Proceed to material loading function
			ProcessNextMaterialChunk(mCurrentChunk);
			break;

		case EDIT3DS:
			// Check mesh version, then proceed to mesh loading function			
			ReadChunk(mTempChunk);
			mTempChunk->mBytesRead += fread(mBuffer, 1, mTempChunk->mLength - mTempChunk->mBytesRead, mFile);
			mCurrentChunk->mBytesRead += mTempChunk->mBytesRead;
			if (mTempChunk->mID != MESHVERSION || *(unsigned short *)mBuffer > 0x03)
				exit(1107);
			ProcessNextChunk(mCurrentChunk);
			break;

		case EDITOBJECT:
			mCurrentChunk->mBytesRead += GetString((char *)mBuffer);
			// mBuffer now contains name of the object to be edited
			ProcessNextObjectChunk(mCurrentChunk);
			break;

		case EDITKEYFRAME:
			ProcessNextKeyFrameChunk(mCurrentChunk);
			break;

		default:  // unrecognized/unsupported chunk
			mCurrentChunk->mBytesRead += fread(mBuffer, 1, mCurrentChunk->mLength - mCurrentChunk->mBytesRead, mFile);
			break;
		}

	aPreviousChunk->mBytesRead += mCurrentChunk->mBytesRead;
	}

	delete mCurrentChunk;
	mCurrentChunk = aPreviousChunk;

	return 1;
}


int Load3ds::ProcessNextObjectChunk(Chunk * aPreviousChunk)
{
	mCurrentChunk = new Chunk;

	while (aPreviousChunk->mBytesRead < aPreviousChunk->mLength)
	{
		ReadChunk(mCurrentChunk);

		switch (mCurrentChunk->mID)
		{
		case OBJTRIMESH:
			// at this point, mBuffer will contain the name of the object being described
			ProcessNextObjectChunk(mCurrentChunk);
			ComputeNormals();

			break;

		case TRIVERT:
			FillVertexBuffer(mCurrentChunk);
			break;

		case TRIFACE:
			FillIndexBuffer(mCurrentChunk);
			break;

		case TRIFACEMAT:
			// your getting a list of triangles that belong to a certain material
			SortIndicesByMaterial(mCurrentChunk);
			break;

		case TRIUV:
			FillTexCoordBuffer(mCurrentChunk);
			break;

		default:  // unrecognized/unsupported chunk
			mCurrentChunk->mBytesRead += fread(mBuffer, 1, mCurrentChunk->mLength - mCurrentChunk->mBytesRead, mFile);
			break;
		}

	aPreviousChunk->mBytesRead += mCurrentChunk->mBytesRead;
	}

	delete mCurrentChunk;
	mCurrentChunk = aPreviousChunk;

	return 1;
}


int Load3ds::ProcessNextMaterialChunk(Chunk * aPreviousChunk)
{
	mCurrentChunk = new Chunk;

	while (aPreviousChunk->mBytesRead < aPreviousChunk->mLength)
	{
		ReadChunk(mCurrentChunk);

		switch (mCurrentChunk->mID)
		{
		case MATNAME:
			mCurrentChunk->mBytesRead += fread(mBuffer, 1, mCurrentChunk->mLength - mCurrentChunk->mBytesRead, mFile);
			break;

		case MATLUMINANCE:
			ReadColorChunk(mCurrentChunk, (float *) mBuffer);
			break;

		case MATDIFFUSE:
			ReadColorChunk(mCurrentChunk, (float *) mBuffer);
			break;

		case MATSPECULAR:
			ReadColorChunk(mCurrentChunk, (float *) mBuffer);
			break;
		
		case MATSHININESS:
			ReadColorChunk(mCurrentChunk, (float *) mBuffer);
			break;
		
		case MATMAP:
			// texture map chunk, proceed
			ProcessNextMaterialChunk(mCurrentChunk);
			break;

		case MATMAPFILE:
			mCurrentChunk->mBytesRead += fread((char *)mBuffer, 1, mCurrentChunk->mLength - mCurrentChunk->mBytesRead, mFile);
			// mBuffer now contains the filename of the next texture; load it if you wish
			break;
		
		default:  // unrecognized/unsupported chunk
			mCurrentChunk->mBytesRead += fread(mBuffer, 1, mCurrentChunk->mLength - mCurrentChunk->mBytesRead, mFile);
			break;
		}

	aPreviousChunk->mBytesRead += mCurrentChunk->mBytesRead;
	}

	delete mCurrentChunk;
	mCurrentChunk = aPreviousChunk;

	return 1;
}


int Load3ds::ProcessNextKeyFrameChunk(Chunk * aPreviousChunk)
{
	mCurrentChunk = new Chunk;

	short int lCurrentID, lCurrentParentID;
	
	while (aPreviousChunk->mBytesRead < aPreviousChunk->mLength)
	{
		ReadChunk(mCurrentChunk);

		switch (mCurrentChunk->mID)
		{
		case KFMESH:
			ProcessNextKeyFrameChunk(mCurrentChunk);
			break;

		case KFHEIRARCHY:
			mCurrentChunk->mBytesRead += fread(&lCurrentID, 1, 2, mFile);
			// lCurrentID now contains the ID of the current object being described
			//  Save this if you want to support an object hierarchy
			break;

		case KFNAME:
			mCurrentChunk->mBytesRead += GetString((char *)mBuffer);
			// mBuffer now contains the name of the object whose KF info will
			//   be described

			mCurrentChunk->mBytesRead += fread(mBuffer, 1, 4, mFile);  // useless, ignore
			mCurrentChunk->mBytesRead += fread(&lCurrentParentID, 1, 2, mFile);
			// lCurrentParentID now contains the ID of the parent of the current object 
			// being described
			break;

		default:  // unrecognized/unsupported chunk
			mCurrentChunk->mBytesRead += fread(mBuffer, 1, mCurrentChunk->mLength - mCurrentChunk->mBytesRead, mFile);
			break;
		}

	aPreviousChunk->mBytesRead += mCurrentChunk->mBytesRead;
	}

	delete mCurrentChunk;
	mCurrentChunk = aPreviousChunk;

	return 1;
}


int Load3ds::GetString(char * aBuffer)
{
	unsigned int lBytesRead = 0;
	int index = 0;

	fread(aBuffer, 1, 1, mFile);

	while (*(aBuffer + index) != 0)
		fread(aBuffer + ++index, 1, 1, mFile);

	return strlen(aBuffer) + 1;
}


int Load3ds::ReadChunk(Chunk * aChunk)
{
	aChunk->mBytesRead = fread(&aChunk->mID, 1, 2, mFile);
	aChunk->mBytesRead += fread(&aChunk->mLength, 1, 4, mFile);

	return 1;
}


int Load3ds::ReadColorChunk(Chunk * aChunk, float * aVector)
{
	ReadChunk(mTempChunk);
	mTempChunk->mBytesRead += fread(mBuffer, 1, mTempChunk->mLength - mTempChunk->mBytesRead, mFile);

	aChunk->mBytesRead += mTempChunk->mBytesRead;
	return 1;
}


int Load3ds::ReadPercentChunk(Chunk * aChunk, float * aPercent)
{
	ReadChunk(mTempChunk);
	mTempChunk->mBytesRead += fread(mBuffer, 1, mTempChunk->mLength - mTempChunk->mBytesRead, mFile);
	*aPercent = (float) *((short int *) mBuffer) / 100.0f;

	aChunk->mBytesRead += mTempChunk->mBytesRead;
	return 1;
}


int Load3ds::FillIndexBuffer(Chunk * aPreviousChunk)
{
	unsigned short int lNumFaces;
	aPreviousChunk->mBytesRead += fread(&lNumFaces, 1, 2, mFile);
	aPreviousChunk->mBytesRead += fread(mBuffer, 1, aPreviousChunk->mLength - aPreviousChunk->mBytesRead, mFile);

	// allocate space for the vertices in the temporary object, and read that information
	// Each face has three indices, corresponding to each face vertex
	mModelData->mFaces = static_cast<int*>( malloc( lNumFaces * sizeof( int ) * 3 ) );
	mModelData->mFaceCount = lNumFaces;
	
	// j is used because we skip the edge flag :P
	// copy just the edges indices and skip the edge flags
	int j = 0;
	for (int i = 0; i < lNumFaces * 4; i+=4, j+=3)
	{
		mModelData->mFaces[ j ] = (static_cast<unsigned short int*>( mBuffer ))[i];
		mModelData->mFaces[ j+1 ] = (static_cast<unsigned short int*>( mBuffer ))[i+1];
		mModelData->mFaces[ j+2 ] = (static_cast<unsigned short int*>( mBuffer ))[i+2];
	}


	//for (int i = 0, j = 0; i < lNumFaces * 4; i+=4, j+=3)
	//{
	//	printf("### Face - X:%d Y:%d Z:%d\n", (static_cast<unsigned short int*>( mBuffer ))[i],
	//		(static_cast<unsigned short int*>( mBuffer ))[i+1],
	//		(static_cast<unsigned short int*>( mBuffer ))[i+2]);
	//	printf("@@@ Face - X:%d Y:%d Z:%d\n", mModelData->mFaces[ j ],
	//		mModelData->mFaces[ j+1 ],
	//		mModelData->mFaces[ j+2 ]);
	//	printf("Mask %d: %d\n", i, (static_cast<unsigned short int*>( mBuffer )[i + 3]));
	//}

	/* 
	 * mBuffer now contains an array of indices (unsigned short ints)
	 * Careful, the list consists of 3 vertex indices and then an edge
	 * flag (safe to ignore, probably
	 * bit 0: CA visible, bit 1: BC visible, bit 2: AB visible
	 */

	return 1;
}


int Load3ds::SortIndicesByMaterial(Chunk * aPreviousChunk)
{
	unsigned short int lNumFaces;
	aPreviousChunk->mBytesRead += GetString((char *) mBuffer);
	// mBuffer contains the name of the material that is associated
	//  with the following triangles (set of 3 indices which index into the vertex list
	//  of the current object chunk)

	aPreviousChunk->mBytesRead += fread(&lNumFaces, 1, 2, mFile);

	aPreviousChunk->mBytesRead += fread(mBuffer, 1, aPreviousChunk->mLength - aPreviousChunk->mBytesRead, mFile);
	// mBuffer now contains a list of triangles that use the material specified above

	return 1;
}


int Load3ds::FillTexCoordBuffer(Chunk * aPreviousChunk)
{
	unsigned short lNumTexCoords;
	aPreviousChunk->mBytesRead += fread(&lNumTexCoords, 1, 2, mFile);
	//aPreviousChunk->mBytesRead += fread(mBuffer, 1, aPreviousChunk->mLength - aPreviousChunk->mBytesRead, mFile);

	// allocate space for the vertices in the temporary object, and read that information
	mModelData->mTexCoords = static_cast<float*>( malloc( aPreviousChunk->mLength - aPreviousChunk->mBytesRead ) );

	// read the information into the object
	aPreviousChunk->mBytesRead += fread(mModelData->mTexCoords, 1, 
		aPreviousChunk->mLength - aPreviousChunk->mBytesRead, mFile);

	mModelData->mContainsTexCoords = true;

	// mBuffer now contains a list of UV coordinates (2 floats)

	return 1;
}


int Load3ds::FillVertexBuffer(Chunk * aPreviousChunk)
{
	unsigned short lNumVertices;
	aPreviousChunk->mBytesRead += fread(&lNumVertices, 1, 2, mFile);
	//aPreviousChunk->mBytesRead += fread(mBuffer, 1, aPreviousChunk->mLength - aPreviousChunk->mBytesRead, mFile);

	// allocate space for the vertices in the temporary object, and read that information
	mModelData->mVertices = static_cast<float*>( malloc( aPreviousChunk->mLength - aPreviousChunk->mBytesRead ) );

	// read the information into the object
	aPreviousChunk->mBytesRead += fread(mModelData->mVertices, 1, 
		aPreviousChunk->mLength - aPreviousChunk->mBytesRead, mFile);
	
	// mModelData->mVertices now contains a list of vertex coordinates (3 floats)

#ifdef GRAPHICS_DEBUG
	// the * 3 is temporary
	FILE* file = NULL;
	file = fopen("vertices.out", "w+");
	assert( file );
	for (int i = 0; i < lNumVertices * 3; i+=3)
	{
		fprintf(file, "%d: X: %f, Y: %f, Z: %f\n", i/3, mModelData->mVertices[i], mModelData->mVertices[i+1], 
			mModelData->mVertices[i+2]);
		//printf("X: %f, Y: %f, Z: %f\n", mModelData->mVertices[i], mModelData->mVertices[i+1], 
		//	mModelData->mVertices[i+2]);
	}
	fclose( file );
#endif
	return 1;
}
void Load3ds::Normalize(float* vec)
{
	float norm = sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);

	vec[0] /= norm;
	vec[1] /= norm;
	vec[2] /= norm;
}

void Load3ds::CrossProduct(float* vecA, float* vecB, float* crossProd)
{
	crossProd[0] = (vecA[1] * vecB[2]) - (vecA[2] * vecB[1]);
	crossProd[1] = (vecA[2] * vecB[0]) - (vecA[0] * vecB[2]);
	crossProd[2] = (vecA[0] * vecB[1]) - (vecA[1] * vecB[0]);;
}

/**
 * @param the vector from a to b
 * @param the first vertex
 * @param the last vertex
 */
void Load3ds::ComputeVectorFromIndices(float* ab, int a, int b)
{
	ab[0] = mModelData->mVertices[ b ] - mModelData->mVertices[ a ];
	ab[1] = mModelData->mVertices[b + 1] - mModelData->mVertices[a + 1];
	ab[2] = mModelData->mVertices[b + 2] - mModelData->mVertices[a + 2];
}

int Load3ds::ComputeNormals()
{
	// use calloc to initialize the space to zeroes so we don't need 
	// to 'clean' the normals before we compute them
	mModelData->mNormals = static_cast<float*>( calloc(mModelData->mFaceCount * 3, sizeof( float )) );

	// the vertices indices
	int a, b, c;
	float ab[3], bc[3], normal[3];
	for (int i = 0; i < mModelData->mFaceCount; i+=3)
	{
		a = mModelData->mFaces[ i ];
		b = mModelData->mFaces[i + 1];
		c = mModelData->mFaces[i + 2];

		// a, b and c should be multiplied by 3 because the vertices are
		// described in a vector by its consecutives coordinates: triple(x, y, z)
		a *= 3;
		b *= 3;
		c *= 3;

		// compute vectors ab and bc and then compute triangle normal
		ComputeVectorFromIndices(ab, a, b);
		ComputeVectorFromIndices(bc, b, c);

		/************************************************************************/
		/* SHOULD I NORMALIZE THE VECTORS??? I COULD USE THE WEIGHT THEY HAVE
		/* OVER THE TRIANGLE'S AREA :)
		/************************************************************************/
		//Normalize( ab );
		//Normalize( bc );
		
		CrossProduct(ab, bc, normal);
		
		for (int j = 0; j < 3; j++)
		{
			mModelData->mNormals[a + j] += normal[ j ];
			mModelData->mNormals[b + j] += normal[ j ];
			mModelData->mNormals[c + j] += normal[ j ];
		}
	}

	// Now, do a second pass by the vertices and normalize the normals
	for (int i = 0; i < mModelData->mFaceCount; i+=3)
	{
		Normalize( &mModelData->mNormals[ i ] );
	}

	return 1;
}


int Load3ds::CompileObjects()
{
	int j = 0;
	TVector3<float> position, normal;
	TVector2<float> texCoord;
	for (int i = 0; i < mModelData->mFaceCount; i++)
	{
		// stride: one face -> 3 positions/normals
		j = i * 3;

		position.setX( mModelData->mVertices[ j ] );
		position.setY( mModelData->mVertices[j + 1] );
		position.setZ( mModelData->mVertices[j + 2] );

		normal.setX( mModelData->mNormals[ j ] );
		normal.setY( mModelData->mNormals[j + 1] );
		normal.setZ( mModelData->mNormals[j + 2] );

		mMesh->addTriangleIndices(mModelData->mFaces[ j ], 
			mModelData->mFaces[j + 1], mModelData->mFaces[j + 2]);
		
		if ( mModelData->mContainsTexCoords )
		{
			// stride: one face -> 2 texture coordinates
			j = i * 2;

			texCoord.setX( mModelData->mTexCoords[ j ] );
			texCoord.setY( mModelData->mTexCoords[j + 1] );
		}

		mMesh->addVertex( TVertex(normal, position, texCoord) );
	}

	return 1;
}


///////////////////////////////////////////////////////////////////
ModelData::ModelData()
{
	this->mVertices = NULL;
	this->mNormals = NULL;
	this->mTexCoords = NULL;
	this->mFaces = NULL;
	this->mFaceCount = 0;
	this->mContainsTexCoords = false;
}


ModelData::~ModelData()
{
	if ( mVertices )
	{
		free( mVertices );
	}

	if ( mNormals )
	{
		free( mNormals );
	}

	if ( mTexCoords )
	{
		free( mTexCoords );
	}

	if ( mFaces )
	{
		free( mFaces );
	}
}


Chunk::Chunk()
{
	mID = 0;
	mLength = 0;
	mBytesRead = 0;
}


Chunk::~Chunk()
{
}