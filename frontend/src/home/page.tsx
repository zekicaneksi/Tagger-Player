import React from 'react';
import { useState } from "react";

import PickLocation from "./PickLocation";
import App from "./App";

const Page = () => {

    const [location, setLocation] = useState<string | null>(null);

    return(
        <>
        {location
            ? <App />
            : <PickLocation setLocation={setLocation}/> 
        }
        </>
    ) 
}

export default Page;
