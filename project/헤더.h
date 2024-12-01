#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>

struct Vertex {
    GLfloat x, y, z;
    glm::vec3 color;
};

struct Face {
    unsigned int v1, v2, v3;
};

struct Model {
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
};

void read_obj_file(const std::string& filename, Model& model) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; // �� �ٰ� �ּ� ����

        std::istringstream iss(line);

        // ���� ������ �Ľ�
        if (line.rfind("v ", 0) == 0) {
            Vertex vertex;
            iss.ignore(2); // "v " �ǳʶ�
            if (!(iss >> vertex.x >> vertex.y >> vertex.z)) {
                throw std::runtime_error("Failed to parse vertex data in file: " + filename);
            }
            model.vertices.push_back(vertex);
        }
        // �� ������ �Ľ�
        else if (line.rfind("f ", 0) == 0) {
            Face face;
            std::string v1_str, v2_str, v3_str;

            iss.ignore(2); // "f " �ǳʶ�
            if (!(iss >> v1_str >> v2_str >> v3_str)) {
                throw std::runtime_error("Failed to parse face data in file: " + filename);
            }

            auto parse_vertex_index = [](const std::string& token) -> unsigned int {
                size_t first_slash = token.find('/');
                if (first_slash == std::string::npos) {
                    // �����ð� ���� ��� (��: "5")
                    return std::stoi(token) - 1; // 1-based �ε����� 0-based�� ��ȯ
                }
                return std::stoi(token.substr(0, first_slash)) - 1; // ������ ���� �κ� ����
                };

            try {
                face.v1 = parse_vertex_index(v1_str);
                face.v2 = parse_vertex_index(v2_str);
                face.v3 = parse_vertex_index(v3_str);

                // �� �߰�
                model.faces.push_back(face);
            }
            catch (const std::exception& e) {
                std::cerr << "Error parsing face line: " << line
                    << ", exception: " << e.what() << std::endl;
            }
        }
    }
}