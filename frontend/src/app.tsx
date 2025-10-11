import React from 'react';
import { createRoot } from 'react-dom/client';

let SelectDirectory = () => {
    window.electronAPI.SelectDirectory().then( (path : void): void | PromiseLike<void> => {
        console.log(path)
        if (path === undefined) {
            console.log("dont open app")
        } else {
            console.log("open app")
        }
    })
}

const root = createRoot(document.body);
root.render(
    <div className="flex items-center justify-center h-screen">
        <div className="flex flex-col items-center justify-center">
            <h1 className="text-[#d85630] font-bold">Welcome!</h1>
            <p className="text-[#d85630]">Please select your music directory</p>
            <button onClick={SelectDirectory}>click me</button>
        </div>
    </div>
);
