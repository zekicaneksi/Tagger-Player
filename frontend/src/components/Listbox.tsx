import React from 'react';
import { useState, SetStateAction } from 'react';

interface listboxElemProps {
    index: number;
    text: string;
    isSelected: boolean;
    setSelectedIndex: React.Dispatch<SetStateAction<number>>;
}

const ListboxElem = (props: listboxElemProps) => {
    return(
        <div className={`w-full border-b ${props.isSelected ? "bg-neutral-400" : "bg-neutral-500"} hover:bg-neutral-400 p-[3px]`} onClick={() => props.setSelectedIndex(props.index)}>
        <p>{props.text}</p>
        </div>
    )
}

interface props {
    elements: string[];
    selectedIndex: number;
    setSelectedIndex: React.Dispatch<SetStateAction<number>>;
}

const Listbox = (props:props) => {
    return(
        <div className="h-full w-full bg-neutral-600 flex flex-col border border-2 border-neutral-500 overflow-y-scroll wrap-break-word">
        {props.elements.map((elem, index) => <ListboxElem key={index} index={index} text={elem} isSelected={props.selectedIndex === index ? true : false} setSelectedIndex={props.setSelectedIndex}/>)}
        </div>
    ) 
}

export default Listbox;
