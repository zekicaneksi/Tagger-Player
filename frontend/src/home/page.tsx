import React from 'react';
import { useState } from "react";

import PickLocation from "./PickLocation";
import App from "./App";

const Page = () => {

    const [locationUuid, setLocationUuid] = useState<string | null>(null);

    return(
        <>
        {locationUuid
            ? <App locationUuid={locationUuid}/>
            : <PickLocation setLocationUuid={setLocationUuid}/> 
        }
        </>
    ) 
}

export default Page;
