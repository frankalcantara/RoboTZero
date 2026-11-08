import { fileURLToPath } from 'node:url';
import { svelte } from '@sveltejs/vite-plugin-svelte';
import { defineConfig } from 'vite';

// SharedArrayBuffer exige cross-origin isolation (docs/03 §6.3). Os mesmos
// headers valem no deploy (E0-07).
const isolation = {
  'Cross-Origin-Opener-Policy': 'same-origin',
  'Cross-Origin-Embedder-Policy': 'require-corp',
};

export default defineConfig({
  plugins: [svelte()],
  // three.js entra por import dinâmico (AP-01) e os artefatos do core precisam
  // ser arquivos reais: o Worker não roda de data: URL.
  build: { assetsInlineLimit: 0 },
  server: {
    headers: isolation,
    // O worker e o .wasm vivem fora de apps/studio: um único dono do artefato.
    fs: { allow: [fileURLToPath(new URL('../..', import.meta.url))] },
  },
  preview: { headers: isolation },
});
