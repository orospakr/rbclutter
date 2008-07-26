;; Emacs lisp code to fetch keysyms from the clutter header
(defun insert-defs ()
  (interactive)
  (let (fn syms)
    (with-temp-buffer
      ;; Get the value of the CFLAGS variable from the Makefile so we
      ;; can extract the include path
      (insert-file-contents "Makefile")
      (unless (re-search-forward "^CFLAGS\\s-+=\\s-+\\(.*\\)$" nil t)
	(error "CFLAGS variable not found in Makefile"))
      ;; Look for the keysyms header in each include path
      (catch 'found
	(dolist (flag (split-string (match-string 1)))
	  (when (and (string-match "\\`-I" flag)
		     (file-exists-p (setq fn (expand-file-name
					      "clutter/clutter-keysyms.h"
					      (substring flag 2)))))
	    (throw 'found nil)))
	(error "clutter/clutter-keysyms.h not found"))
      ;; Look for each constant in the header
      (erase-buffer)
      (insert-file-contents fn)
      (while (re-search-forward (concat "^\\s-*#define\\s-+CLUTTER_"
					"\\([A-Z0-9a-z_]+\\)"
					"\\s-+0x[0-9a-f]+\\s-*$") nil t)
	(push (match-string 1) syms))
      (setq syms (nreverse syms)))
    ;; Make a string containing all of the symbol names
    (insert "static const char rbclt_keysyms_names[] =\n"
	    "  \"")
    (let ((col 3))
      (dolist (def syms)
	(when (> (+ (length def) 8 col) 80)
	  (setq col 3)
	  (insert "\"\n  \""))
	(setq col (+ col (length def) 6))
	(insert "Key_" def "\\0")))
    ;; Make an array of VALUEs containing the symbol values
    (insert "\";\n"
	    "\n"
	    "static const VALUE rbclt_keysyms_values[] =\n"
	    "  {\n"
	    "    ")
    (let ((col 4) v (first t))
      (dolist (def syms)
	(if first
	    (setq first nil)
	  (insert ", ")
	  (setq col (+ 2 col)))
	(setq v (concat "INT2FIX (CLUTTER_" def ")"))
	(when (> (+ (length v) col 2) 80)
	  (setq col 4)
	  (insert "\n    "))
	(setq col (+ col (length v)))
	(insert v)))
    (insert "\n  };")))
