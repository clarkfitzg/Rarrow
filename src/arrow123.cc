
/* Wed Jul 26 10:35:24 PDT 2017
 *
 * Goal: build an integer vector with Arrow and bring it to R
 * Following Getting Started section in https://arrow.apache.org/docs/cpp/
 */


#include <arrow/api.h>

// Can include this, just needs to be after std library includes that
// happen in Arrow
//#include <Rdefines.h>

#include <R.h>
#include <Rinternals.h>


using namespace arrow;

const int32_t* arrow123;

void init()
{    
// Calling the class constructor for Int32Builder
    arrow::Int32Builder builder(arrow::default_memory_pool(), arrow::int32());

    builder.Append(1);
    builder.Append(2);
    builder.Append(3);

    std::shared_ptr<arrow::Array> array;
    builder.Finish(&array);

        // Cast the Array to its actual type to access its data
    std::shared_ptr<Int32Array> int32_array = std::static_pointer_cast<Int32Array>(array);

      // Get the pointer to the actual data
    arrow123 = int32_array->raw_values();
}


extern "C" 
SEXP c_get123()
{
    int n = 3;

    init();

    // Create the new return object from within C
    SEXP out = PROTECT(allocVector(INTSXP, n));

    // C pointer to the actual data
    int *outp = INTEGER(out);

    // Using C std library to copy memory, Arrow may provide better way
    memcpy(outp, arrow123, n * sizeof(int));

    UNPROTECT(1);
    return out;
}

