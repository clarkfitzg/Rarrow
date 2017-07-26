
/* Wed Jul 26 10:35:24 PDT 2017
 *
 * Goal: build an integer vector with Arrow and bring it to R
 * Following Getting Started section in https://arrow.apache.org/docs/cpp/
 */


#include <arrow/api.h>
#include <arrow/builder.h>

// Calling the class constructor for Int32Builder
arrow::Int32Builder builder(arrow::default_memory_pool(), arrow::int32());

builder.Append(1);
builder.Append(2);
builder.Append(3);

std::shared_ptr<Array> array;
builder.Finish(&array);

/*
// Cast the Array to its actual type to access its data
std::shared_ptr<Int32Array> int32_array = std::static_pointer_cast<Int32Array>(array);

// Get the pointer to the actual data
const int32_t* arrow123 = int32_array->raw_data();
*/
