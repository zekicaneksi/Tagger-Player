import React from 'react';
import { useState } from 'react';

import Listbox from '../components/Listbox'

interface props {
    locationUuid: string
}

const App = (props: props) => {

    const [tagListboxIndex, setTagListboxIndex] = useState<number>(-1);
    
    const elemArr = [
        "oompa",
        "lumpppa",
        "dubidu",
        "diip",
        "give",
        "me",
        "chocolateeeeeeeee iiiiiiiiii ooooooooooooo hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh",
        "cohocooooooooooooooooooooooooooooooooooooooooooos",
        "please",
        "willy",
        "wonka",
        "thankyou"
    ]

    return (
        <>
        <div className="w-sm h-50">
        <Listbox
        elements={elemArr}
        selectedIndex={tagListboxIndex}
        setSelectedIndex={setTagListboxIndex}
        /> 
        </div>

        <p>Selected element is: {tagListboxIndex === -1 ? "no element is selected" : elemArr[tagListboxIndex]}</p>

        <div>
        <p>Hello from app, uuid is: {props.locationUuid}</p>
        </div>
        </>
    )
}

export default App;
