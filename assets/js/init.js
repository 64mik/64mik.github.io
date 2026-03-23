(function() 
    { const saved = localStorage.getItem("theme"); 
        if (saved === "light" || saved === null) 
            { document.documentElement.classList.add("light"); } 
    }
)();