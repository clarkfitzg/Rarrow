library(RCIndex)
library(RCodeGen)


arrow_dir = "/Users/clark/dev/arrow/cpp/src"

tu = createTU("arrow.cc", includes = arrow_dir, args = "-std=c++11")

# Couple warnings, but runs
e = getEnums(tu)

#system.time(k <- getCppClasses(tu, "arrow/.*"))
#
## 631
#length(k)


#cat(makeEnumClass(e$PageSegMode), file = "../R/PageSegMode.R", sep = "\n")
cat(unlist(lapply(e, makeEnumClass)), file = "../R/arrow_enums.R", sep = "\n\n")

# makeEnumDef(e$PageSegMode, namespace = "tesseract"),
ccode = lapply(e, function(e) {
                    p = getCursorSemanticParent(e@def)
                    makeEnumDef(e, namespace = if(p$kind == CXCursor_Namespace) "arrow" else character())
                  })

cat('#include "Rarrow.h"', "", unlist(ccode),
   sep = "\n", file = "../src/arrow_enums.cc")
