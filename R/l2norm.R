l2norm = function(x)
{
    .Call(c_l2norm, as.numeric(x))
}
