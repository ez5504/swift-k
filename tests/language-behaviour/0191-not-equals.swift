type messagefile {}

(messagefile t) greeting(boolean m) { 
    app {
        echo m stdout=@filename(t);
    }
}

messagefile outfile <"0191-not-equals.out">;

boolean i = 7!=9;

outfile = greeting(i);
