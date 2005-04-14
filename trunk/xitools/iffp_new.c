/*
CAT FORMs:
 No nesting allowed! Basically just an archive format.
 Standard way of storing Xiqual files for distribution?

LIST FORMs:
 May contain many types of FORMs, and PROP chunks for each
 FORM type at the very top.
 Nesting allowed:
 FORM LIST
  PROP ILBM size w,h
  LIST ILBM
   PROP ILBM depth 8
   ILBM ...
   ILBM ...
  LIST ILBM depth 16
   ILBM ...
   ILBM ...


PROP chunks: Only find at top of a FORM LIST
 PROP "ILBM" means properties are for all FORMs of type ILBM.
 Only one PROP chunk per FORM type is allowed.
*/
