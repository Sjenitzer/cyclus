/* relaxngvalidator.cpp
 * this class is modeled off of the schemavalidator in libxml++
 * here is their license statement:
 * 
 * libxml++ and this file are copyright (C) 2000 by Ari Johnson,
 * (C) 2002-2004 by the libxml dev team and
 * are covered by the GNU Lesser General Public License, which should be
 * included with libxml++ as the file COPYING.
 */

#include "RelaxNGValidator.h"
#include <libxml/xmlerror.h>
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
RelaxNGValidator::RelaxNGValidator() : schema_(0), valid_context_(0) {}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
RelaxNGValidator::~RelaxNGValidator() {
  release_underlying();
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
void RelaxNGValidator::parse_context(xmlRelaxNGParserCtxtPtr context) {
  release_underlying(); // Free any existing info
  schema_ = xmlRelaxNGParse( context );
  if ( !schema_ )
   throw CycParseException("Schema could not be parsed");
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
void RelaxNGValidator::parse_memory(const Glib::ustring& contents) {
  xmlRelaxNGParserCtxtPtr context = 
    xmlRelaxNGNewMemParserCtxt( contents.c_str(), contents.bytes() );
  parse_context( context );
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
void RelaxNGValidator::release_underlying() {
  if(valid_context_) {
    xmlRelaxNGFreeValidCtxt( valid_context_ );
    valid_context_ = 0;
  }

  if(schema_) {
    xmlRelaxNGFree(schema_);
    schema_ = 0;
  }
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
bool RelaxNGValidator::validate(const xmlpp::Document* doc) {
  if (!doc)
    throw CycValidityException("Document pointer cannot be 0");

  if (!schema_)
    throw CycValidityException("Must have a schema to validate document");

  // A context is required at this stage only
  if (!valid_context_)
    valid_context_ = xmlRelaxNGNewValidCtxt( schema_ );

  if(!valid_context_)
    throw CycValidityException("Couldn't create validating context");
  
  int res;
  try {
    res = xmlRelaxNGValidateDoc( valid_context_, (xmlDocPtr)doc->cobj() );
  } catch (xmlError e) {
    throw CycValidityException(e.message);
  }
  
  if(res != 0)
    throw CycValidityException("Document failed schema validation");

  return res;
}
