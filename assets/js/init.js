(function() 
    { const saved = localStorage.getItem("theme"); 
        if (saved === "dark" || saved === null) 
            { document.documentElement.classList.add("dark"); } 
    }
)();