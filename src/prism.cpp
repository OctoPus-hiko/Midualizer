#include <glm/geometric.hpp> //cross product, normalization
#include <glm/vec3.hpp>
#include <range/v3/view/concat.hpp>

#include "prism.h"

using std::vector;



vector<float> prism(int n){
    const float pi = std::numbers::pi_v<float>;

    // bottom coners
    vector<vector<float>> bottom_corners;
    for(int i = 0; i <= n - 1; i++){
        const float x = cos(2 * pi * i / n + pi / 2);
        const float y = sin(2 * pi * i / n + pi / 2);

        const vector<float> corner = {x, y, 0.0f};
        bottom_corners.emplace_back(corner);
    }

    // top corners
    vector<vector<float>> top_corners;
    for(vector<float> bottom_corner : bottom_corners){
        const vector<float> top_corner = {bottom_corner[0], bottom_corner[1], 1.0f};
        top_corners.emplace_back(top_corner);
    }





    // side vertices with normals
    vector<vector<float>> side_vertices_normals;
    for(int i = 0; i <= n - 2; i++){
        // Calculate the normal
        const glm::vec3 vectorAB(
            bottom_corners[i + 1][0] - bottom_corners[i][0],
            bottom_corners[i + 1][1] - bottom_corners[i][1],
            bottom_corners[i + 1][2] - bottom_corners[i][2]
        );
        const glm::vec3 vectorBC(
            top_corners[i][0] - bottom_corners[i + 1][0],
            top_corners[i][1] - bottom_corners[i + 1][1],
            top_corners[i][2] - bottom_corners[i + 1][2]
        );

        const glm::vec3 cross_product = glm::cross(vectorAB, vectorBC);
        const glm::vec3 glm_normal = glm::normalize(cross_product);
        const vector<float> normal = {glm_normal.x, glm_normal.y, glm_normal.z};

        // first triangle
        side_vertices_normals.emplace_back(bottom_corners[i]);
        side_vertices_normals.emplace_back(normal);
        side_vertices_normals.emplace_back(bottom_corners[i + 1]);
        side_vertices_normals.emplace_back(normal);
        side_vertices_normals.emplace_back(top_corners[i]);
        side_vertices_normals.emplace_back(normal);

        // second triangle
        side_vertices_normals.emplace_back(bottom_corners[i + 1]);
        side_vertices_normals.emplace_back(normal);
        side_vertices_normals.emplace_back(top_corners[i + 1]);
        side_vertices_normals.emplace_back(normal);
        side_vertices_normals.emplace_back(top_corners[i]);
        side_vertices_normals.emplace_back(normal);
    }



    // last side
    const glm::vec3 vectorDE(
        bottom_corners[0][0] - bottom_corners[n - 1][0],
        bottom_corners[0][1] - bottom_corners[n - 1][1],
        bottom_corners[0][2] - bottom_corners[n - 1][2]
    );
    const glm::vec3 vectorEF(
        top_corners[n - 1][0] - bottom_corners[0][0],
        top_corners[n - 1][1] - bottom_corners[0][1],
        top_corners[0][2] - bottom_corners[0][2]
    );

    const glm::vec3 cross_product = glm::cross(vectorDE, vectorEF);
    const glm::vec3 glm_normal = glm::normalize(cross_product);
    const vector<float> normal = {glm_normal.x, glm_normal.y, glm_normal.z};

    side_vertices_normals.emplace_back(bottom_corners[n - 1]);
    side_vertices_normals.emplace_back(normal);
    side_vertices_normals.emplace_back(bottom_corners[0]);
    side_vertices_normals.emplace_back(normal);
    side_vertices_normals.emplace_back(top_corners[n - 1]);
    side_vertices_normals.emplace_back(normal);

    side_vertices_normals.emplace_back(bottom_corners[0]);
    side_vertices_normals.emplace_back(normal);
    side_vertices_normals.emplace_back(top_corners[0]);
    side_vertices_normals.emplace_back(normal);
    side_vertices_normals.emplace_back(top_corners[n - 1]);
    side_vertices_normals.emplace_back(normal);





    // bottom vertices without normals
    vector<vector<float>> bottom_vertices;
    // add the three vertices of each triangle
    for(int i = 0; i <= n - 2; i++){
        bottom_vertices.emplace_back(vector<float> {0.0f, 0.0f, 0.0f});
        bottom_vertices.emplace_back(bottom_corners[i]);
        bottom_vertices.emplace_back(bottom_corners[i + 1]);
    }

    // last triangle
    bottom_vertices.emplace_back(vector<float> {0.0f, 0.0f, 0.0f});
    bottom_vertices.emplace_back(bottom_corners[n - 1]);
    bottom_vertices.emplace_back(bottom_corners[0]);



    // top vertices
    vector<vector<float>> top_vertices;
    for(vector<float> bottom_vertex : bottom_vertices){
        const vector<float> top_vertex = {bottom_vertex[0], bottom_vertex[1], 1.0f};
        top_vertices.emplace_back(top_vertex);
    }



    // Combine top and bottom vertices
    const auto bottom_top_vertices = ranges::views::concat(bottom_vertices, top_vertices);

    // Add normals
    vector<vector<float>> bottom_top_vertices_normals;
    const vector<float> bottom_top_normal = {0.0f, 0.0f, 1.0f};
    for(vector<float> vertex : bottom_top_vertices){
        bottom_top_vertices_normals.emplace_back(vertex);
        bottom_top_vertices_normals.emplace_back(bottom_top_normal);
    }





    // Combine all the vertices
    const auto vertices = ranges::views::concat(bottom_top_vertices_normals, side_vertices_normals);
    
    // Flatten the vector
    vector<float> flat_vertices;
    for(vector<float> vertex : vertices){
      flat_vertices.emplace_back(vertex[0]);
      flat_vertices.emplace_back(vertex[1]);
      flat_vertices.emplace_back(vertex[2]);
    }

    return flat_vertices;
}
