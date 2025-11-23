import React from 'react';
import { useState, useEffect } from 'react';

import Listbox from '../components/Listbox'
import Loading from "../components/Loading"

interface props {
    locationUuid: string
}

const App = (props: props) => {

    const [loading, setLoading] = useState<boolean>(true);
    const [errMsg, setErrMsg] = useState<string>("");

    const [newTagText, setNewTagText] = useState<string>("");

    const [tagListbox, setTagListbox] = useState<string[]>([]);
    const [tagListboxIndex, setTagListboxIndex] = useState<number>(-1);

    // A helper function to make api requests
    const MakeApiRequest = async (args: {
        path: string;
        method: "GET" | "POST" | "PUT" | "DELETE";
        body?: any;
    }) => {
        const response = await window.electronAPI.ApiRequest({
            path: args.path,
            headers: {"X-uuid": props.locationUuid},
            method: args.method,
            body: args.body
        });

        if(response.status === undefined) {
            setErrMsg("Problem reaching the backend. Make sure it is running and accessible.")
        } else if (response.status === 401) {
            setErrMsg(response.data.error)
        } else {
            return response;
        }
    }

    useEffect(() => {
        const getTags = async () => {
            const response = await MakeApiRequest({
                path: "/get-tags",
                method: "GET"
            });

            setLoading(false);
            if(response) {
                if (response.success){
                    setTagListbox(response.data.tags);
                } else {
                    setErrMsg(response.data.error);
                }
            }
        }

        getTags();
    }, [])

    const addTagBtn = async () => {

        if (newTagText.length === 0 || newTagText.length >= 200) return;

        setLoading(true);

        const response = await MakeApiRequest({
            path: "/add-tag",
            method: "POST",
            body: {tag: newTagText}
        });

        setLoading(false);
        if (response) {
            if (response.success){
                setTagListbox(oldArr => [...oldArr, newTagText]);
            } else {
                setErrMsg(response.data.error);
            }
        }
    }

    return (
        <>
        <Loading loading={loading}>
        {errMsg !== "" &&
            <div className="fixed flex items-center justify-center inset-0 z-50 bg-black/60">
        <div className="flex flex-col items-center justify-center rounded-md p-10 bg-neutral-500">
        <p className="text-cyan-50">{errMsg}</p>
        <p className="mt-8 font-light text-sm text-slate-200">{"It is best to restart the app"}</p>
        <button className="mt-2 bg-stone-600 p-2 pl-8 pr-8 font-bold text-cyan-50 hover:cursor-pointer" onClick={() => window.electronAPI.quitApp()}>Quit</button>
        </div>
        </div>}
        <input value={newTagText} onChange={e => setNewTagText(e.target.value)} className="bg-neutral-300"/>
        <button className={`bg-stone-500 p-1 ${newTagText.length === 0 || newTagText.length >= 200 ? "opacity-50 cursor-not-allowed" : "hover:cursor-pointer"}`} onClick={addTagBtn}>add</button>
        <div className="w-sm h-50">
        <Listbox
        elements={tagListbox}
        selectedIndex={tagListboxIndex}
        setSelectedIndex={setTagListboxIndex}
        /> 
        </div>
        <p>Selected element is: {tagListboxIndex === -1 ? "no element is selected" : tagListbox[tagListboxIndex]}</p>
        </Loading>
        </>
    )
}

export default App;
