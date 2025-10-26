import React from 'react';
import { SetStateAction } from 'react';

import FolderIcon from '../asset/icon/folder-open.svg';

interface props {
    setLocation: React.Dispatch<SetStateAction<string | null>>
}

const Page = (props: props) => {

    let SelectDirectory = () => {
        window.electronAPI.SelectDirectory().then( (path: string | undefined) => {
            if (path !== undefined) {
                props.setLocation(path)
            }
        })
    }

    return(
        <div className="flex items-center justify-center h-screen">
        <div className="flex flex-col items-center justify-center">
        <h1 className="text-[#d85630] font-bold">Welcome!</h1>
        <p className="text-[#d85630]">Please select your music directory</p>
        <FolderIcon
        className="w-16 h-auto fill-amber-500 hover:fill-amber-300 transition-colors duration-1000 cursor-pointer"
        onClick={SelectDirectory}
        />
        </div>
        </div>
    ) 
}

export default Page;
