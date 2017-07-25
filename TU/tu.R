library(RCIndex)
library(RCodeGen)


arrow_dir = "/Users/clark/dev/arrow/cpp/src"

tu = createTU("arrow.cc", includes = arrow_dir, args = "-std=c++11")

# Couple warnings, but runs
e = getEnums(tu)

# Need to go home, then I'll run this for longer.
system.time(k <- getCppClasses(tu))


#cat(makeEnumClass(e$PageSegMode), file = "../R/PageSegMode.R", sep = "\n")
cat(unlist(lapply(e, makeEnumClass)), file = "../R/PageSegMode.R", sep = "\n")

# makeEnumDef(e$PageSegMode, namespace = "tesseract"),
ccode = lapply(e, function(e) {
                    p = getCursorSemanticParent(e@def)
                    makeEnumDef(e, namespace = if(p$kind == CXCursor_Namespace) "tesseract" else character())
                  })
cat('#include "Rtesseract.h"',
    '#include "RConverters.h"', "",
    unlist(ccode),
   sep = "\n", file = "../src/PageSegMode.cc")
