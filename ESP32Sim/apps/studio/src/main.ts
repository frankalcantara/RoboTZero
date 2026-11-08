import { mount } from 'svelte';
import App from './App.svelte';
import { t } from './i18n';
import './styles/tokens.css';

document.title = t('app.title');
mount(App, { target: document.getElementById('app')! });
