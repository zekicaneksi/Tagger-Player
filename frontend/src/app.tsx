import React from 'react';
import { createRoot } from 'react-dom/client';
import { createHashRouter } from "react-router";
import { RouterProvider } from "react-router";

import HomePage from "./home/page";

const router = createHashRouter([
    {
        path: "/",
        element: <HomePage />,
    },
]);

const root = createRoot(document.body);

root.render(
    <RouterProvider router={router} />,
);
