import { writable } from 'svelte/store';

/**
 * UI state store - manages interface state
 */
export const uiStore = writable({
  activeTab: 'track',
  sidebarOpen: true,
  theme: 'dark',
  showGrid: true,
  showStats: false
});

/**
 * Modal state store
 */
export const modalStore = writable({
  isOpen: false,
  type: null, // 'save', 'load', 'export', 'import', 'settings'
  data: null
});

/**
 * Notification store
 */
export const notificationStore = writable({
  visible: false,
  message: '',
  type: 'info' // 'info', 'success', 'warning', 'error'
});

/**
 * Show notification
 * @param {string} message - Notification message
 * @param {string} type - Notification type (info, success, warning, error)
 * @param {number} duration - Auto-hide duration in ms (0 = no auto-hide)
 */
export function showNotification(message, type = 'info', duration = 3000) {
  notificationStore.set({
    visible: true,
    message,
    type
  });

  if (duration > 0) {
    setTimeout(() => {
      notificationStore.update(n => ({ ...n, visible: false }));
    }, duration);
  }
}

/**
 * Hide notification
 */
export function hideNotification() {
  notificationStore.update(n => ({ ...n, visible: false }));
}

/**
 * Open modal
 * @param {string} type - Modal type
 * @param {any} data - Optional data to pass to modal
 */
export function openModal(type, data = null) {
  modalStore.set({
    isOpen: true,
    type,
    data
  });
}

/**
 * Close modal
 */
export function closeModal() {
  modalStore.set({
    isOpen: false,
    type: null,
    data: null
  });
}
