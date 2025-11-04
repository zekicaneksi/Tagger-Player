import React from 'react';
import { SetStateAction, useState } from 'react';

import Loading from "../components/Loading"

import FolderIcon from '../asset/icon/folder-open.svg';

interface props {
    setLocationUuid: React.Dispatch<SetStateAction<string | null>>
}

const Page = (props: props) => {

    const [loading, setLoading] = useState<boolean>(false);
    const [errMsg, setErrMsg] = useState<string>("");


    let SelectDirectory = () => {
        window.electronAPI.SelectDirectory().then( async (path: string | undefined) => {
            if (path !== undefined) {
                setLoading(true);

                const response = await window.electronAPI.ApiRequest({
                    path: "/pick-path",
                    method: "POST",
                    body: { path: path },
                });

                if (response.success) {
                    props.setLocationUuid(response.data.uuid)
                } else {
                    if(response.status === undefined) {
                        setErrMsg("Problem reaching the backend. Make sure it is running and accessible.")
                    }
                    else {
                        setErrMsg(response.status + ": " + response.error);
                    }
                    setLoading(false);
                }
            }
        })
    }

    return(
        <Loading loading={loading}>
        <div className="flex items-center justify-center h-screen">
        <div className="flex flex-col items-center justify-center">
        {errMsg !== "" && <p className="text-red-400 font-bold mb-[20%]">{errMsg}</p>}
        <h1 className="text-[#d85630] font-bold">Welcome!</h1>
        <p className="text-[#d85630]">Please select your music directory</p>
        <FolderIcon
        className="w-16 h-auto fill-amber-500 hover:fill-amber-300 transition-colors duration-1000 cursor-pointer"
        onClick={SelectDirectory}
        />
        </div>
        </div>
        </Loading>
    ) 
}

export default Page;
