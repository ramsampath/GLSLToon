#include "CMesh.h"

CMesh::CMesh()
: iVertexCount( 0 )
{
}

CMesh::~CMesh(void)
{
}

/**
 * @param aLocation
 * @description sets the binormal location on the shader
 */
void CMesh::setBinormalLocation(GLuint aLocation)
{
	this->iVertexBinormalLoc = aLocation;
}

/**
 * @param aLocation
 * @description sets the tangent location on the shader
 */
void CMesh::setTangentLocation(GLuint aLocation)
{
	this->iVertexTangentLoc = aLocation;
}

/**
 *
 */
void CMesh::draw()
{
	// Enable a vertex position, normal and texture coords arrays, respectively
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	//glEnableClientState( GL_TEXTURE_COORD_ARRAY );  
	
	// Enable special attributes: Tangent and Binormal arrays
	//glEnableVertexAttribArray( this->iVertexTangentLoc );
	//glEnableVertexAttribArray( this->iVertexBinormalLoc );
	
	// Passing the information to openGL using pointers to the actual arrays
	glVertexPointer( 3, GL_FLOAT, sizeof(TVertex), &(this->iVertices[0].iPosition.x()) );
	glNormalPointer( GL_FLOAT, sizeof(TVertex), &(this->iVertices[0].iNormal.x()) );
	//glTexCoordPointer( 2, GL_FLOAT, sizeof(TVertex), &(this->iVertices[0].iTexCoord));	
	//glVertexAttribPointer(this->iVertexTangentLoc, 3, GL_FLOAT, 0, sizeof(TVertex), 
	//	&(this->iVertices[0].iTangent));
	//glVertexAttribPointer(this->iVertexBinormalLoc, 3, GL_FLOAT, 0, sizeof(TVertex), 
	//	&(this->iVertices[0].iTangent));

	// the actual drawing
	glDrawElements(GL_TRIANGLES, this->iVertexCount, GL_UNSIGNED_INT, &(this->iIndices.front()));

	//glDisableVertexAttribArray( this->iVertexBinormalLoc );
	//glDisableVertexAttribArray( this->iVertexTangentLoc );
	glDisableClientState( GL_VERTEX_ARRAY );	
	glDisableClientState( GL_NORMAL_ARRAY );	
	//glDisableClientState( GL_TEXTURE_COORD_ARRAY );
}