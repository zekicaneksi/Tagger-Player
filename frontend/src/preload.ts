// See the Electron documentation for details on how to use preload scripts:
// https://www.electronjs.org/docs/latest/tutorial/process-model#preload-scripts

import {contextBridge, ipcRenderer} from 'electron'

contextBridge.exposeInMainWorld('electronAPI', {
    SelectDirectory: () => ipcRenderer.invoke('dialog:openDirectory'),
    ApiRequest: (args: {
        path: string;
        method: "GET" | "POST" | "PUT" | "DELETE";
        body?: any;
        headers?: Record<string, string>;
    }) => ipcRenderer.invoke("api-request", args),
    quitApp: () => ipcRenderer.send('quit-app'),
})
