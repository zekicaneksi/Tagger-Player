export interface IElectronAPI {
  SelectDirectory: () => Promise<void>,
}

declare global {
  interface Window {
    electronAPI: IElectronAPI
  }
}
