;; Ruby bindings for the Clutter 'interactive canvas' library.
;; Copyright (C) 2007  Neil Roberts
;; 
;; This library is free software; you can redistribute it and/or
;; modify it under the terms of the GNU Lesser General Public
;; License as published by the Free Software Foundation; either
;; version 2.1 of the License, or (at your option) any later version.
;; 
;; This library is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;; Lesser General Public License for more details.
;; 
;; You should have received a copy of the GNU Lesser General Public
;; License along with this library; if not, write to the Free Software
;; Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
;; MA  02110-1301  USA
;; 'mkmethod' is an Emacs lisp function that I found useful to make a
;; stub for a method definition. It edits the '_init' function to add
;; a call to rb_define_method and makes a skeleton function definition
;; for the method.

(defun mkmethod-capitalize-class-name (class-name)
  (apply 'concat (mapcar 'capitalize (split-string class-name "_"))))

(defun mkmethod (method-name)
  "Make a stub function for the method METHOD-NAME.
The method calls the clutter function with the same name and no
parameters and then returns SELF."
  (interactive "MMethod name: ")
  (goto-char (point-max))
  (unless (re-search-backward "^void *\nrbclt_\\([a-zA-Z_]+\\)_init\\>" nil t)
    (error "Class initializer not found"))
  (let ((class-name (match-string 1)) save-point end-init)
    (setq end-init (match-beginning 0))
    (goto-char (point-max))
    (unless (re-search-backward (concat "^  rb_define_method "
					"(klass, \"[^\"\n]*\",[[:space:]\n]+"
					"rbclt_"
					(regexp-quote class-name)
					"_[a-zA-Z0-9_]+, -?[0-9]+);") nil t)
      (error "Previous method definition not found"))
    (goto-char (match-end 0))
    (insert "\n")
    (let ((parta (concat "  rb_define_method (klass, \"" method-name "\","))
	  (partb (concat "rbclt_" class-name "_" method-name ", 0);")))
      ;; Split the line into two if it won't fit in 80 characters
      (if (<= (+ (length parta) (length partb) 1) 80)
	  (insert parta " " partb)
	(insert parta "\n" (make-string 20 ? ) partb)))
    (goto-char end-init)
    (insert "static VALUE\n"
	    "rbclt_" class-name "_" method-name " (VALUE self)\n"
	    "{\n"
	    "  Clutter" (mkmethod-capitalize-class-name class-name) " *"
	    class-name " = CLUTTER_" (upcase class-name)
	    " (RVAL2GOBJ (self));\n\n")
    (setq save-point (point))
    (insert "  clutter_" class-name "_" method-name " (" class-name ");\n\n"
	    "  return self;\n"
	    "}\n"
	    "\n")
    (goto-char save-point)
    )
  )
