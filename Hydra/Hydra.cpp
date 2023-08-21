//-----------------------------------------------------------------------------
// Entaro ChucK Developer!
// This is a Chugin boilerplate, generated by chuginate!
//-----------------------------------------------------------------------------

/* TODO LIST
   - [DONE] pass in args to python program (string interpolation?)
   - [DONE] take in the python output as std in (after converting to json)
   - [DONE] parse in as json
   - [DONE] make python script a string instead of a file
   - [TODO] Handle types
     - [DONE] String
     - [DONE] Int
     - [DONE] Float
     - [DONE] Bool
     - [TODO] (P1) Array
       - only return list of Hydra
     - [TODO] (P1) Dur 
       - parse string as Dur
     - [TODO] (P2) time 
     - [TODO] (P2) complex
     - [TODO] (P2) polar
     - [TODO] (P2) vec3 
     - [TODO] (P2) vec4 
   - [TODO] is_nil() - now to handle?
   - [TODO] set() 
     - take in type, return Hydra
   - [TODO] handle error case: if json conversion fails, print error and return nil
   - [TODO] outputs dir
     - make outputs dir (mkdir -p): ./outputs/YYYY-MM-DD/HH-MM-SS/
       - alternatively, have hydra make this and pass it as metadata (it already will?)
   - [TODO] maybe have more structured dict
     - hydra type
       - [DONE] hydra.get("key"): returns Hydra if contents is another hydra struct, error if it's a value
       - hydra.int(): returns int if current Hydra object contains a value, error on
         missing or type conversion
       - hydra.float() etc...
       - ex: hydra.get("foo").get("bar").int() => int a;
     - hydra.dir() - get proper output dir
   - pass args override from cmd line (see if I can do this automatically)
     - chuck hydra.ck:foo=2:bar=4
   - [TODO] proper error handling
     - [TODO] handle get_*() failure (print to stderr, return null/0/"")
     - [TODO] check if python is installed
     - [TODO] check if hydra is installed (print pip message to run otherwise)
     - [TODO] gracefully handle parse failure
 */

// this should align with the correct versions of these ChucK files
#include "chuck_dl.h"
#include "chuck_def.h"

// general includes
#include <stdio.h>
#include <limits.h>
#include <iostream>
#include <variant>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

// The python program to be run. This is embedded here rather than
// as a file as it means I don't have to manage where the script is
// relative to the chugin (which seems tricky to do)
std::string config_init = R"(
import json
import sys

from hydra import compose, initialize
from omegaconf import OmegaConf

# context initialization
with initialize(version_base=None, config_path=sys.argv[1]):
    cfg = compose(config_name=sys.argv[2], overrides=[])
    container = OmegaConf.to_container(cfg, resolve=True)
    print(json.dumps(container))
)";

// declaration of chugin constructor
CK_DLL_CTOR(hydra_ctor);
// declaration of chugin desctructor
CK_DLL_DTOR(hydra_dtor);

// example of getter/setter
CK_DLL_MFUN(hydra_setParam);
CK_DLL_MFUN(hydra_getParam);

CK_DLL_MFUN(hydra_init);
CK_DLL_MFUN(hydra_get);
CK_DLL_MFUN(hydra_get_str);
CK_DLL_MFUN(hydra_get_int);
CK_DLL_MFUN(hydra_get_float);
CK_DLL_MFUN(hydra_get_bool);

CK_DLL_MFUN(hydra_is_null);

// this is a special offset reserved for Chugin internal data
t_CKINT hydra_data_offset = 0;


// class definition for internal Chugin data
// (note: this isn't strictly necessary, but serves as example
// of one recommended approach)
class Hydra
{
private:
  // The values in a hydra config are either YAML values or a
  // map of hydra configs. Here configs are recusively defined.
  using value_type = std::variant
    <std::monostate,
     std::map<std::string, Hydra*>,
     std::string,
     double,
     bool,
     std::monostate // use monostate for null
     >;
  value_type value;

public:
  // constructor
  Hydra()
  {
    // initialize empty map
    std::map<std::string, Hydra*> val;
    value = val;
  }

  Hydra(std::string val) {
    value = val;
  }

  Hydra(double val) {
    value = val;
  }

  Hydra(bool val) {
    value = val;
  }

  // Monotype means this is null.
  Hydra(std::monostate val) {
  }

  // set parameter example
  t_CKFLOAT setParam( t_CKFLOAT p )
  {
    m_param = p;
    return p;
  }

  // get parameter example
  t_CKFLOAT getParam() { return m_param; }

  // take in:
  // - config_path
  // - config_name
  // - overrides (list of strings) (later)
  //
  // will pass things into python program and call
  // the dirs
  void init(std::string config_path, std::string config_name) {
    // This is hacky but what are ya gonna do?
    std::string cmd = "python -c \"" + config_init + "\" " + config_path + " " + config_name;
    std::string result = exec(cmd);

    // TODO add a try catch block here to handle parse error
    auto j = json::parse(result);

    this->build_tree(j);
  }

  void build_tree(json j) {
    std::cout << "dump: " << j.dump() << std::endl;
    // get map from variant
    std::map values = std::get<1>(value);
    
    for (auto& element : j.items()) {
      auto val = element.value();
      auto key = element.key();
      std::cout << "val: " << val << std::endl;
      // build out object
      if (val.is_object()) {
        // figure this out
        /*
          - make a new hydra (default to empty map)
          - call new_hyra.build_tree
          - add to map
          - done
         */
        Hydra * elem = new Hydra();
        elem->build_tree(val);
        values[key] = elem;
      } else if (val.is_string()) {
        std::string str_val = val.template get<std::string>();
        Hydra * elem = new Hydra(str_val);
        values[key] = elem;
      } else if (val.is_number()) {
        double num_val = val.template get<double>();
        Hydra * elem = new Hydra(num_val);
        values[key] = elem;
      } else if (val.is_boolean()) {
        bool bool_val = val.template get<bool>();
        Hydra * elem = new Hydra(bool_val);
        values[key] = elem;
      } else if (val.is_null()) {
        Hydra * elem = new Hydra(std::monostate{});
        values[key] = elem;
      }
    }

    value = values;
  }

  void set(std::string key, Hydra* val) {
    std::map values = std::get<1>(value);
    values[key] = val;
    value = values;
  }

  // Get hydra value to be transformed into a hydra class
  Hydra* get(std::string key) {
    std::map values = std::get<1>(value);
    return values[key];
  }

  std::string get_string() {
    std::string val = std::get<std::string>(value);
    return val;
  }

  t_CKINT get_int() {
    t_CKINT val = (int)std::get<double>(value);
    return val;
  }

  t_CKFLOAT get_float() {
    t_CKFLOAT val = std::get<double>(value);
    return val;
  }

  t_CKINT get_bool() {
    t_CKINT val = (int)std::get<bool>(value);
    return val;
  }

  t_CKINT is_null() {
    // check if variant is in monostate
    if(value.index() == 0) {
      return true;
    }

    return false;
  }

  // fetcha string from key. Either return string or raise an
  // exception.
  // std::string getString(std::string key) {

  // }
    
private:
  // instance data
  t_CKFLOAT m_param;

  // execute cmd and return the stdout as a string
  // see the link for how to deal with windows
  // https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po
  std::string exec(std::string cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
      throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
      result += buffer.data();
    }
    return result;
  }
};


// query function: chuck calls this when loading the Chugin
// NOTE: developer will need to modify this function to
// add additional functions to this Chugin
CK_DLL_QUERY( Hydra )
{
    // hmm, don't change this...
    QUERY->setname(QUERY, "Hydra");
    
    // begin the class definition
    // can change the second argument to extend a different ChucK class
    QUERY->begin_class(QUERY, "Hydra", "UGen");

    // register the constructor (probably no need to change)
    QUERY->add_ctor(QUERY, hydra_ctor);
    // register the destructor (probably no need to change)
    QUERY->add_dtor(QUERY, hydra_dtor);

    // example of adding setter method
    QUERY->add_mfun(QUERY, hydra_setParam, "float", "param");
    // example of adding argument to the above method
    QUERY->add_arg(QUERY, "float", "arg");

    // example of adding getter method
    QUERY->add_mfun(QUERY, hydra_getParam, "float", "param");

    // init method
    QUERY->add_mfun(QUERY, hydra_init, "void", "init");
    QUERY->add_arg(QUERY, "string", "config_path");
    QUERY->add_arg(QUERY, "string", "config_name");

    QUERY->add_mfun(QUERY, hydra_get, "Hydra", "get");
    QUERY->add_arg(QUERY, "string", "key");

    QUERY->add_mfun(QUERY, hydra_get_str, "string", "get_string");
    QUERY->add_mfun(QUERY, hydra_get_int, "int", "get_int");
    QUERY->add_mfun(QUERY, hydra_get_float, "float", "get_float");
    QUERY->add_mfun(QUERY, hydra_get_bool, "int", "get_bool");
    QUERY->add_mfun(QUERY, hydra_is_null, "int", "is_null");
    
    // this reserves a variable in the ChucK internal class to store 
    // referene to the c++ class we defined above
    hydra_data_offset = QUERY->add_mvar(QUERY, "int", "@h_data", false);

    // end the class definition
    // IMPORTANT: this MUST be called!
    QUERY->end_class(QUERY);

    // wasn't that a breeze?
    return TRUE;
}

// implementation for the constructor
CK_DLL_CTOR(hydra_ctor)
{
  // get the offset where we'll store our internal c++ class pointer
  OBJ_MEMBER_INT(SELF, hydra_data_offset) = 0;
    
  // instantiate our internal c++ class representation
  Hydra * h_obj = new Hydra();
    
  // store the pointer in the ChucK object member
  OBJ_MEMBER_INT(SELF, hydra_data_offset) = (t_CKINT) h_obj;
}


// implementation for the destructor
CK_DLL_DTOR(hydra_dtor)
{
    // get our c++ class pointer
    Hydra * h_obj = (Hydra *) OBJ_MEMBER_INT(SELF, hydra_data_offset);
    // check it
    if( h_obj )
    {
        // clean up
        delete h_obj;
        OBJ_MEMBER_INT(SELF, hydra_data_offset) = 0;
        h_obj = NULL;
    }
}


// example implementation for setter
CK_DLL_MFUN(hydra_setParam)
{
  // get our c++ class pointer
  Hydra * h_obj = (Hydra *) OBJ_MEMBER_INT(SELF, hydra_data_offset);
  // set the return value
  RETURN->v_float = h_obj->setParam(GET_NEXT_FLOAT(ARGS));
}


// example implementation for getter
CK_DLL_MFUN(hydra_getParam)
{
    // get our c++ class pointer
    Hydra * h_obj = (Hydra *) OBJ_MEMBER_INT(SELF, hydra_data_offset);
    // set the return value
    RETURN->v_float = h_obj->getParam();
}


CK_DLL_MFUN(hydra_init)
{
  // get our c++ class pointer
  Hydra * h_obj = (Hydra *) OBJ_MEMBER_INT(SELF, hydra_data_offset);

  std::string config_path = GET_NEXT_STRING_SAFE(ARGS);
  std::string config_name = GET_NEXT_STRING_SAFE(ARGS);

  h_obj->init(config_path, config_name);
}


CK_DLL_MFUN(hydra_get)
{
    // get our c++ class pointer
    Hydra * h_obj = (Hydra *) OBJ_MEMBER_INT(SELF, hydra_data_offset);

    std::string key = GET_NEXT_STRING_SAFE(ARGS);

    Hydra * val = h_obj->get(key);

    // Allocate a Hydra object and return it
    Chuck_DL_Api::Object obj = API->object->create(API, SHRED, API->object->get_type(API, SHRED, "Hydra"));
    Chuck_Object * object = (Chuck_Object *) obj;
    OBJ_MEMBER_INT(object, hydra_data_offset) = (t_CKINT) val;

    RETURN->v_object = object;
}

CK_DLL_MFUN(hydra_get_str)
{
    // get our c++ class pointer
    Hydra * h_obj = (Hydra *) OBJ_MEMBER_INT(SELF, hydra_data_offset);

    std::string val = h_obj->get_string();
    RETURN->v_string = (Chuck_String*)API->object->create_string(API, SHRED, val.c_str());
}

CK_DLL_MFUN(hydra_get_int)
{
    // get our c++ class pointer
    Hydra * h_obj = (Hydra *) OBJ_MEMBER_INT(SELF, hydra_data_offset);

    RETURN->v_int = h_obj->get_int();;
}

CK_DLL_MFUN(hydra_get_float)
{
    // get our c++ class pointer
    Hydra * h_obj = (Hydra *) OBJ_MEMBER_INT(SELF, hydra_data_offset);

    RETURN->v_float = h_obj->get_float();;
}

CK_DLL_MFUN(hydra_get_bool)
{
    // get our c++ class pointer
    Hydra * h_obj = (Hydra *) OBJ_MEMBER_INT(SELF, hydra_data_offset);

    RETURN->v_int = h_obj->get_bool();;
}

CK_DLL_MFUN(hydra_is_null)
{
    // get our c++ class pointer
    Hydra * h_obj = (Hydra *) OBJ_MEMBER_INT(SELF, hydra_data_offset);

    RETURN->v_int = h_obj->is_null();;
}
