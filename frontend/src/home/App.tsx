import React from 'react';

interface props {
    locationUuid: string
}

const App = (props: props) => {
    return (
        <div>
        <p>Hello from app, uuid is: {props.locationUuid}</p>
        </div>
    )
}

export default App;
