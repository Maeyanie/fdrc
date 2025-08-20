#include <future>

#include "loader.h"
#include "vertex.h"

#include <draco/compression/decode.h>
#include <draco/io/mesh_io.h>
#include <draco/mesh/mesh.h>
using namespace std;


Loader::Loader(QObject* parent, const QString& filename, bool is_reload)
    : QThread(parent), filename(filename), is_reload(is_reload)
{
    // Nothing to do here
}

void Loader::run()
{
    Mesh* mesh = load_drc();
    if (mesh)
    {
        if (mesh->empty())
        {
            emit error_empty_mesh();
            delete mesh;
        }
        else
        {
            emit got_mesh(mesh, is_reload);
            emit loaded_file(filename);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void parallel_sort(Vertex* begin, Vertex* end, int threads)
{
    if (threads < 2 || end - begin < 2)
    {
        std::sort(begin, end);
    }
    else
    {
        const auto mid = begin + (end - begin) / 2;
        if (threads == 2)
        {
            auto future = std::async(parallel_sort, begin, mid, threads / 2);
            std::sort(mid, end);
            future.wait();
        }
        else
        {
            auto a = std::async(std::launch::async, parallel_sort, begin, mid, threads / 2);
            auto b = std::async(std::launch::async, parallel_sort, mid, end, threads / 2);
            a.wait();
            b.wait();
        }
        std::inplace_merge(begin, mid, end);
    }
}

Mesh* mesh_from_verts(uint32_t tri_count, QVector<Vertex>& verts)
{
    // Save indicies as the second element in the array
    // (so that we can reconstruct triangle order after sorting)
    for (size_t i=0; i < tri_count*3; ++i)
    {
        verts[i].i = i;
    }

    // Check how many threads the hardware can safely support. This may return
    // 0 if the property can't be read so we shoud check for that too.
    auto threads = std::thread::hardware_concurrency();
    if (threads == 0)
    {
        threads = 8;
    }

    // Sort the set of vertices (to deduplicate)
    parallel_sort(verts.begin(), verts.end(), threads);

    // This vector will store triangles as sets of 3 indices
    std::vector<GLuint> indices(tri_count*3);

    // Go through the sorted vertex list, deduplicating and creating
    // an indexed geometry representation for the triangles.
    // Unique vertices are moved so that they occupy the first vertex_count
    // positions in the verts array.
    size_t vertex_count = 0;
    for (auto v : verts)
    {
        if (!vertex_count || v != verts[vertex_count-1])
        {
            verts[vertex_count++] = v;
        }
        indices[v.i] = vertex_count - 1;
    }
    verts.resize(vertex_count);

    std::vector<GLfloat> flat_verts;
    flat_verts.reserve(vertex_count*3);
    for (auto v : verts)
    {
        flat_verts.push_back(v.x);
        flat_verts.push_back(v.y);
        flat_verts.push_back(v.z);
    }

    return new Mesh(std::move(flat_verts), std::move(indices));
}

////////////////////////////////////////////////////////////////////////////////

Mesh* Loader::load_drc()
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        emit error_missing_file();
        return NULL;
    }

	return read_drc(file);
}

Mesh* Loader::read_drc(QFile& file)
{
    uchar* data = file.map(0, file.size());
	if (!data) {
		emit error_missing_file();
		return NULL;
	}

    draco::DecoderBuffer buffer;
    buffer.Init((const char*)data, file.size());

    /*const draco::EncodedGeometryType geom_type = draco::Decoder::GetEncodedGeometryType(&buffer);
    if (geom_type != draco::TRIANGULAR_MESH) {
		emit error_bad_stl();
		return NULL;
    }*/

    draco::Decoder decoder;
    draco::Mesh mesh;
    draco::Status status = decoder.DecodeBufferToGeometry(&buffer, &mesh);
    if (!status.ok()) {
        emit error_bad_stl();
        return NULL;
    }
	
	QVector<Vertex> tris;
	size_t faces = mesh.num_faces();
	tris.reserve(faces * 3);

    const draco::PointAttribute *const poses = mesh.GetNamedAttribute(draco::GeometryAttribute::POSITION);

    float pos[3];
    for (draco::FaceIndex i(0); i < faces; i++) {
        draco::Mesh::Face face = mesh.face(i);

        for (int j = 0; j < 3; j++) {
            draco::AttributeValueIndex posIndex = poses->mapped_index(face[j]);
            poses->ConvertValue<float>(posIndex, 3, pos);

            Vertex vert(pos[0], pos[1], pos[2]);
            tris.push_back(vert);
        }
	}

    return mesh_from_verts(faces, tris);
}


