type file;

app (file out, file err) remote_driver (file args)
{
    cat @args stdout=filename(out) stderr=filename(err);
}

file driver_out <simple_mapper; prefix="sanity", suffix=".out">;
file driver_err <simple_mapper; prefix="sanity", suffix=".err">;
file t <"http://idkoru.com/hello.txt">;

(driver_out, driver_err) = remote_driver(t);
