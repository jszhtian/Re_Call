#include	"tonad3d3_pch.h"

#include	<fstream>

namespace NT3
{
	// Vertex format
	struct VERTEX
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 normal;
		D3DXVECTOR2 texcoord;
	};

	static bool load_mesh_from_obj(CTSTR& file_path, ARRAY<VERTEX>& vertex_list, ARRAY<DWORD>& index_list)
	{
	    TCHAR str_command[256] = {0};
		ARRAY<D3DXVECTOR3> position_list;
		ARRAY<D3DXVECTOR2> texcoord_list;
		ARRAY<D3DXVECTOR3> normal_list;

		// 結果をクリア
		vertex_list.clear();
		index_list.clear();

		// ファイルを開く
		std::wifstream in_file(file_path);
		if (!in_file)	{
			set_last_error(_T("メッシュファイルが開けませんでした。"), _T("load_mesh_from_obj"));
			return false;
		}
		else	{
		
			while (1)	{

				in_file >> str_command;
				if (!in_file)
					break;

				if (_tcscmp(str_command, _T("#")) == 0)
				{
					// Comment
				}
				else if (_tcscmp(str_command, _T("v")) == 0)
				{
					// Vertex Position
					float x, y, z;
					in_file >> x >> y >> z;
					position_list.push_back(D3DXVECTOR3(x, y, z)); 
				}
				else if (_tcscmp(str_command, _T("vt")) == 0)
				{
					// Vertex TexCoord
					float u, v;
					in_file >> u >> v;
					texcoord_list.push_back(D3DXVECTOR2(u, v)); 
				}
				else if (_tcscmp(str_command, _T("vn")) == 0)
				{
					// Vertex Normal
					float x, y, z;
					in_file >> x >> y >> z;
					normal_list.push_back(D3DXVECTOR3(x, y, z));
				}
				else if (_tcscmp(str_command, _T("f")) == 0)
				{
					// Face
					UINT i_position, i_texcoord, i_normal;
					VERTEX vertex;
            
					for (int i_face = 0; i_face < 3; i_face++)
					{
						ZeroMemory(&vertex, sizeof(VERTEX));

						// OBJ format uses 1-based arrays
						in_file >> i_position;
						vertex.position = position_list[i_position - 1];
                
						if ('/' == in_file.get())
						{
							// Optional texture coordinate
							in_file >> i_texcoord;
							vertex.texcoord = texcoord_list[i_texcoord - 1];

							if ('/' == in_file.get())
							{
								// Optional vertex normal
								in_file >> i_normal;
								vertex.normal = normal_list[i_normal - 1];
							}
						}

						// If a duplicate vertex doesn't exist, add this vertex to the Vertices
						// list. Store the index in the Indices array. The Vertices and Indices
						// lists will eventually become the Vertex Buffer and Index Buffer for
						// the mesh.
						index_list.push_back(vertex_list.size());
						vertex_list.push_back(vertex);
					}
					//m_Attributes.Add( dwCurSubset );
				}
			}
		}

		return true;
	}
}

