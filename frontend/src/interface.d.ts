export interface IElectronAPI {
  SelectDirectory: () => Promise<undefined | string>,
}

declare global {
  interface Window {
    electronAPI: IElectronAPI
  }
}
