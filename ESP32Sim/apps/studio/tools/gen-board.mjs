// Gera public/board/c3-devkitm1.gltf: uma caixa unitária reutilizada por dois
// nós (placa e LED), com escala em milímetros e origem no canto da placa
// (docs/03 §7.3). Asset autoral, sem cor: a tinta vem dos tokens (AP-07).
// Rode com `node tools/gen-board.mjs`.
import { mkdirSync, writeFileSync } from 'node:fs';
import { dirname, join } from 'node:path';
import { fileURLToPath } from 'node:url';

const output = join(
  dirname(fileURLToPath(import.meta.url)),
  '..',
  'public',
  'board',
  'c3-devkitm1.gltf',
);

// Cantos em sentido anti-horário visto de fora.
const faces = [
  { normal: [0, 0, 1], corners: [[-0.5, -0.5, 0.5], [0.5, -0.5, 0.5], [0.5, 0.5, 0.5], [-0.5, 0.5, 0.5]] },
  { normal: [0, 0, -1], corners: [[0.5, -0.5, -0.5], [-0.5, -0.5, -0.5], [-0.5, 0.5, -0.5], [0.5, 0.5, -0.5]] },
  { normal: [0, 1, 0], corners: [[-0.5, 0.5, 0.5], [0.5, 0.5, 0.5], [0.5, 0.5, -0.5], [-0.5, 0.5, -0.5]] },
  { normal: [0, -1, 0], corners: [[-0.5, -0.5, -0.5], [0.5, -0.5, -0.5], [0.5, -0.5, 0.5], [-0.5, -0.5, 0.5]] },
  { normal: [1, 0, 0], corners: [[0.5, -0.5, 0.5], [0.5, -0.5, -0.5], [0.5, 0.5, -0.5], [0.5, 0.5, 0.5]] },
  { normal: [-1, 0, 0], corners: [[-0.5, -0.5, -0.5], [-0.5, -0.5, 0.5], [-0.5, 0.5, 0.5], [-0.5, 0.5, -0.5]] },
];

const positions = [];
const normals = [];
const indices = [];
for (const { normal, corners } of faces) {
  const base = positions.length / 3;
  for (const corner of corners) {
    positions.push(...corner);
    normals.push(...normal);
  }
  indices.push(base, base + 1, base + 2, base, base + 2, base + 3);
}

const positionBytes = positions.length * 4;
const normalBytes = normals.length * 4;
const buffer = Buffer.alloc(positionBytes + normalBytes + indices.length * 2);
positions.forEach((value, i) => buffer.writeFloatLE(value, i * 4));
normals.forEach((value, i) => buffer.writeFloatLE(value, positionBytes + i * 4));
indices.forEach((value, i) => buffer.writeUInt16LE(value, positionBytes + normalBytes + i * 2));

const min = [0, 1, 2].map((axis) => Math.min(...positions.filter((_, i) => i % 3 === axis)));
const max = [0, 1, 2].map((axis) => Math.max(...positions.filter((_, i) => i % 3 === axis)));

const gltf = {
  asset: {
    version: '2.0',
    generator: 'ESP32Sim tools/gen-board.mjs',
    copyright: 'Autoral ESP32Sim; ver provenance/board-esp32-c3-devkitm-1.yaml',
  },
  scene: 0,
  scenes: [{ name: 'ESP32-C3-DevKitM-1', nodes: [0, 1] }],
  nodes: [
    // 25,4 x 1,6 x 17,8 mm, canto inferior na origem.
    { name: 'Board', mesh: 0, translation: [12.7, 0.8, 8.9], scale: [25.4, 1.6, 17.8] },
    { name: 'LED', mesh: 0, translation: [10, 2.1, 8], scale: [3, 1, 3] },
  ],
  meshes: [
    {
      name: 'UnitBox',
      primitives: [{ attributes: { POSITION: 0, NORMAL: 1 }, indices: 2, material: 0 }],
    },
  ],
  // Branco neutro: o renderer aplica a cor dos tokens.
  materials: [
    { name: 'Tint', pbrMetallicRoughness: { baseColorFactor: [1, 1, 1, 1], metallicFactor: 0, roughnessFactor: 0.8 } },
  ],
  accessors: [
    { bufferView: 0, componentType: 5126, count: 24, type: 'VEC3', min, max },
    { bufferView: 1, componentType: 5126, count: 24, type: 'VEC3' },
    { bufferView: 2, componentType: 5123, count: indices.length, type: 'SCALAR' },
  ],
  bufferViews: [
    { buffer: 0, byteOffset: 0, byteLength: positionBytes, target: 34962 },
    { buffer: 0, byteOffset: positionBytes, byteLength: normalBytes, target: 34962 },
    { buffer: 0, byteOffset: positionBytes + normalBytes, byteLength: indices.length * 2, target: 34963 },
  ],
  buffers: [{ byteLength: buffer.length, uri: `data:application/octet-stream;base64,${buffer.toString('base64')}` }],
};

mkdirSync(dirname(output), { recursive: true });
writeFileSync(output, `${JSON.stringify(gltf, null, 2)}\n`);
console.log(`escrito ${output} (${buffer.length} bytes de geometria)`);
