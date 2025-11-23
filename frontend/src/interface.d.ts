export interface IElectronAPI {
    SelectDirectory: () => Promise<undefined | string>,
    ApiRequest: (args: {
        path: string;
        method: "GET" | "POST" | "PUT" | "DELETE";
        body?: any;
        headers?: Record<string, string>;
    }) => Promise<{ success: boolean; status?: number; data?: any; error?: string }>,
    quitApp: () => void;
}

declare global {
    interface Window {
        electronAPI: IElectronAPI
    }
}
