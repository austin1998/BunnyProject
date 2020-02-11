#include <iostream>
#include <string>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "Image.h"

// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;

//finds area of triangles
float areaTri(float x1, float y1, float x2, float y2, float x3, float y3) {
	return abs((x1*(y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2);
}

int main(int argc, char **argv)
{
	if(argc < 2) {
		cout << "Usage: A1 meshfile" << endl;
		return 0;
	}
	string meshName(argv[1]);

	string fileName(argv[2]);
	// Width of image
	int width = atoi(argv[3]);
	// Height of image
	int height = atoi(argv[4]);
	int colormode = atoi(argv[5]);
	// Create the image
	auto image = make_shared<Image>(width + 1, height);

	// Load geometry
	vector<float> posBuf; // list of vertex positions
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	string errStr;
	bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());
	if(!rc) {
		cerr << errStr << endl;
	} else {
		// Some OBJ files have different indices for vertex positions, normals,
		// and texture coordinates. For example, a cube corner vertex may have
		// three different normals. Here, we are going to duplicate all such
		// vertices.
		// Loop over shapes
		for(size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces (polygons)
			size_t index_offset = 0;
			for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				size_t fv = shapes[s].mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for(size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+0]);
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+1]);
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+2]);
				}
				index_offset += fv;
				// per-face material (IGNORE)
				shapes[s].mesh.material_ids[f];
			}
		}
	}
	cout << "Number of vertices: " << posBuf.size()/3 << endl;


	float xGMin = posBuf.at(0);
	float xGMax = posBuf.at(0);
	float yGMin = posBuf.at(1);
	float yGMax = posBuf.at(1);
	float zGMin = posBuf.at(2);
	float zGMax = posBuf.at(2);
	vector<vector<float>> zBuf(width, vector<float> (height, -1));

	for (int i = 0; i < posBuf.size(); i = i + 3) {
		//X
		if (xGMin > posBuf.at(i)) {
			xGMin = posBuf.at(i);
		}
		if (xGMax < posBuf.at(i)) {
			xGMax = posBuf.at(i);
		}
		//Y
		if (yGMin > posBuf.at(i + 1)) {
			yGMin = posBuf.at(i + 1);
		}
		if (yGMax < posBuf.at(i + 1)) {
			yGMax = posBuf.at(i + 1);
		}
		//Z
		if (zGMin > posBuf.at(i + 2)) {
			zGMin = posBuf.at(i + 2);
		}
		if (zGMax < posBuf.at(i + 2)) {
			zGMax = posBuf.at(i + 2);
		}
		
	}

	//set scale
	float scale = height / (yGMax - yGMin);
	if ((height / (yGMax - yGMin)) < (width / (xGMax - xGMin))) {
		scale = height / (yGMax - yGMin);
	}
	else {
		scale = width / (xGMax - xGMin);
	}

	//set translation
	float xTranslat = (0.5 * width - (scale * 0.5 * (xGMax + xGMin)));
	float yTranslat = (.5 * height - (scale * 0.5 * (yGMax + yGMin)));

	zGMax = zGMax - zGMin;

	int x, y;
	float xMin, xMax, yMin, yMax;
	int SXMin, SXMax, SYMin, SYMax;
	float u, v, w, z1, z2, z3;
	int x1, x2, x3, y1, y2, y3, red1, green1, blue1, red2, green2, blue2, red3, green3, blue3, red, green, blue;

	//for each triangle 
	for (int i = 0; i < posBuf.size(); i += 9) {
		//reset the x and y bound
		xMin = posBuf.at(i);
		xMax = posBuf.at(i);
		yMin = posBuf.at(i+1);
		yMax = posBuf.at(i+1);
		//for each vertex
		for (int j = i; j < i + 9; j += 3) {
			//X
			if (xMin > posBuf.at(j)) {
				xMin = posBuf.at(j);
			}
			if (xMax < posBuf.at(j)) {
				xMax = posBuf.at(j);
			}
			//Y
			if (yMin > posBuf.at(j + 1)) {
				yMin = posBuf.at(j + 1);
			}
			if (yMax < posBuf.at(j + 1)) {
				yMax = posBuf.at(j + 1);
			}
			
			if (colormode == 0) {
				red1 = rand() % 255;
				red2 = rand() % 255;
				red3 = rand() % 255;
				green1 = rand() % 255;
				green2 = rand() % 255;
				green3 = rand() % 255;
				blue1 = rand() % 255;
				blue2 = rand() % 255;
				blue3 = rand() % 255;
			}

		}
		//scale and translate
		SXMin = (scale * xMin) + xTranslat;
		SXMax = (scale * xMax) + xTranslat;
		SYMin = (scale * yMin) + yTranslat;
		SYMax = (scale * yMax) + yTranslat;
		//for each pixle //could be <=
		for (int j = SXMin; j < SXMax; j++) {
			for (int k = SYMin; k < SYMax; k++) {
				x1 = (scale * posBuf.at(i)) + xTranslat;
				y1 = (scale * posBuf.at(i + 1)) + yTranslat;
				x2 = (scale * posBuf.at(i + 3)) + xTranslat;
				y2 = (scale * posBuf.at(i + 4)) + yTranslat;
				x3 = (scale * posBuf.at(i + 6)) + xTranslat;
				y3 = (scale * posBuf.at(i + 7)) + yTranslat;

				z1 = posBuf.at(i + 2) + zGMin;
				z2 = posBuf.at(i + 5) + zGMin;
				z3 = posBuf.at(i + 8) + zGMin;

				u = areaTri(x3, y3, x1, y1, j, k) / areaTri(x1, y1, x2, y2, x3, y3);
				v = areaTri(x1, y1, x2, y2, j, k) / areaTri(x1, y1, x2, y2, x3, y3);
				w = areaTri(x2, y2, x3, y3, j, k) / areaTri(x1, y1, x2, y2, x3, y3);
				if ((u >= 0 && u <= 1) && (v >= 0 && v <= 1) && (w >= 0 && w <= 1)) {
					if ((j >= floor(w * x1 + u * x2 + v * x3) && (j <= ceil(w * x1 + u * x2 + v * x3))) || (k >= floor(w * y1 + u * y2 + v * y3) && k >= ceil(w * y1 + u * y2 + v * y3))) {
						//coloring
						if (colormode == 0) {
							red = (w*red1 + u * red2 + v * red3);
							green = (w*green1 + u * green2 + v * green3);
							blue = (w*blue1 + u * blue2 + v * blue3);
							image->setPixel(j, k, red, green, blue);
						}else if (colormode == 1) {
							if (zBuf.at(j).at(k) < (w*z1 + u * z2 + v * z3) / zGMax) {
								zBuf.at(j).at(k) = (w*z1 + u * z2 + v * z3) / zGMax;
								image->setPixel(j, k, zBuf.at(j).at(k) * 255, u * 0, v * 0);
							}
						}
						else if (colormode == 2) {
							image->setPixel(j, k, 255, 255 - (255 * ((float)k/height)), 255 - (255 * ((float) k/height)));
						}
						else {
							cout << "Not a valid Colormode" << endl;
						}
					}
					
				}
			}
		}
	}

	image->writeToFile(fileName);
	return 0;
}
