// import * as THREE from './three';

let width = 500;
let height = 281;

let scene = new THREE.Scene();
let camera = new THREE.PerspectiveCamera(75, width / height, 0.1, 1000);

let renderer = new THREE.WebGLRenderer();
renderer.setSize(width, height);
document.body.appendChild(renderer.domElement);

let wing_geometry = new THREE.Geometry();
wing_geometry.vertices.push(
    new THREE.Vector3(0, 0, 0), //0
    new THREE.Vector3(6, 0, 0),
    new THREE.Vector3(2, 10, 0),
    new THREE.Vector3(0, 10, 0),

    new THREE.Vector3(0, 0, 1), //4
    new THREE.Vector3(0, 10, 1)
);
wing_geometry.faces.push(
    new THREE.Face3(0, 3, 2),
    new THREE.Face3(0, 2, 1),

    new THREE.Face3(4, 1, 2),
    new THREE.Face3(4, 2, 5),

    new THREE.Face3(0, 1, 4),
    new THREE.Face3(3, 5, 2),

    new THREE.Face3(0, 4, 3),
    new THREE.Face3(3, 4, 5),
);
wing_geometry.computeBoundingSphere();
var wing_edges = new THREE.EdgesGeometry(wing_geometry);

let plane_geometry = new THREE.Geometry();
plane_geometry.vertices.push(
    new THREE.Vector3(0, 0, 0), //0
    new THREE.Vector3(10, 0, 0),
    new THREE.Vector3(0, 20, 0),
    new THREE.Vector3(10, 20, 0),

    new THREE.Vector3(0, 0, 5), //4
    new THREE.Vector3(10, 0, 5),

    new THREE.Vector3(0, 5, 10), //6
    new THREE.Vector3(10, 5, 10),
    new THREE.Vector3(0, 20, 10),
    new THREE.Vector3(10, 20, 10),

    new THREE.Vector3(0, 5, 0), //10
    new THREE.Vector3(10, 5, 0)
);
plane_geometry.faces.push(
    new THREE.Face3(3, 0, 2),
    new THREE.Face3(0, 3, 1),

    new THREE.Face3(9, 8, 6),
    new THREE.Face3(6, 7, 9),

    new THREE.Face3(0, 5, 4),
    new THREE.Face3(0, 1, 5),

    new THREE.Face3(4, 7, 6),
    new THREE.Face3(4, 5, 7),

    new THREE.Face3(2, 8, 9),
    new THREE.Face3(2, 9, 3),

    new THREE.Face3(11, 9, 7),
    new THREE.Face3(11, 3, 9),

    new THREE.Face3(10, 6, 8),
    new THREE.Face3(10, 8, 2),

    new THREE.Face3(1, 11, 5),
    new THREE.Face3(5, 11, 7),

    new THREE.Face3(0, 4, 10),
    new THREE.Face3(6, 10, 4)
);
plane_geometry.computeBoundingSphere();
var plane_edges = new THREE.EdgesGeometry(plane_geometry);

let material = new THREE.MeshBasicMaterial({
    color: 0x008800,
    wireframe: false
});

let plane = new THREE.Mesh(plane_geometry, material);
let wing_left = new THREE.Mesh(wing_geometry, material);
let wing_right = new THREE.Mesh(wing_geometry, material);

var plane_line = new THREE.LineSegments(
    plane_edges,
    new THREE.LineBasicMaterial({ color: 0x000000 })
);
var wing_line_left = new THREE.LineSegments(
    wing_edges,
    new THREE.LineBasicMaterial({ color: 0x000000 })
);
scene.add(plane);
plane.add(plane_line);
plane.add(wing_left);
wing_left.add(wing_line);
wing_left.rotation.z = -3.141592 / 2;
wing_left.position.x = 10;
wing_left.position.y = 10;
wing_left.position.z = 3;

camera.position.z = 50;

function animate() {
    requestAnimationFrame(animate);
    // plane.rotation.x += 0.02;
    plane.rotation.y += 0.02;
    renderer.render(scene, camera);
}
animate();
