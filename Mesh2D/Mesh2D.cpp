//-----------------------------------------------------------------------------
// Entaro ChucK Developer!
// This is a Chugin boilerplate, generated by chuginate!
//-----------------------------------------------------------------------------

// this should align with the correct versions of these ChucK files
#include "chuck_dl.h"
#include "chuck_def.h"

// general includes
#include <stdio.h>
#include <limits.h>
#include "Mesh2D-stk.h"

namespace stk {
// declaration of chugin constructor
CK_DLL_CTOR(mesh2d_ctor);
// declaration of chugin desctructor
CK_DLL_DTOR(mesh2d_dtor);

// example of getter/setter
CK_DLL_MFUN(mesh2d_noteOn);
CK_DLL_MFUN(mesh2d_noteOff);
CK_DLL_MFUN(mesh2d_setNX);
CK_DLL_MFUN(mesh2d_getNX);
CK_DLL_MFUN(mesh2d_setNY);
CK_DLL_MFUN(mesh2d_getNY);
CK_DLL_MFUN(mesh2d_setXpos);
CK_DLL_MFUN(mesh2d_getXpos);
CK_DLL_MFUN(mesh2d_setYpos);
CK_DLL_MFUN(mesh2d_getYpos);
CK_DLL_MFUN(mesh2d_setDecay);
CK_DLL_MFUN(mesh2d_getDecay);

// for Chugins extending UGen, this is mono synthesis function for 1 sample
CK_DLL_TICK(mesh2d_tick);

// this is a special offset reserved for Chugin internal data
t_CKINT mesh2d_data_offset = 0;


// class definition for internal Chugin data
// (note: this isn't strictly necessary, but serves as example
// of one recommended approach)
class Mesh2D
{
public:
  // constructor
  Mesh2D( t_CKFLOAT fs)
  {
    m_mesh = new Mesh2D_stk(5,4);
    _x = 5;
    _y = 4;
    _xpos = 0;
    _ypos = 0;
    _decay = 0.999f;
	m_mesh->setInputPosition(_xpos,_ypos);
	m_mesh->setDecay(_decay);
  }
  
  // for Chugins extending UGen
  SAMPLE tick( SAMPLE in)
  {
    return m_mesh->tick(in);
  }
  
  // set parameter example
  int setNX( t_CKINT p )
  {
    _x = p;
    m_mesh->setNX(_x);
    return p;
  }
  
  // get parameter example
  float getNX() { return _x; }

  // set parameter example
  int setNY( t_CKINT p )
  {
    _y = p;
    m_mesh->setNY(_y);
    return p;
  }
  
  // get parameter example
  float getNY() { return _y; }

  // set parameter example
  float setXpos( t_CKFLOAT p )
  {
    _xpos = p;
    m_mesh->setInputPosition(_xpos, _ypos);
    return p;
  }
  
  // get parameter example
  float getXpos() { return _xpos; }

  // set parameter example
  float setYpos( t_CKFLOAT p )
  {
    _ypos = p;
    m_mesh->setInputPosition(_xpos, _ypos);
    return p;
  }
  
  // get parameter example
  float getYpos() { return _ypos; }

  // set parameter example
  float setDecay( t_CKFLOAT p )
  {
    _decay = p;
    m_mesh->setDecay(_decay);
    return p;
  }
  
  // get parameter example
  float getDecay() { return _decay; }

  float noteOn( t_CKFLOAT p)
  {
    m_mesh->noteOn(0,p);
    return p;
  }

  float noteOff( t_CKFLOAT p)
  {
    m_mesh->noteOff(p);
    return p;
  }
  
private:
  // instance data
  int _x, _y;
  float _xpos, _ypos;
  float _decay;
  Mesh2D_stk* m_mesh;
};
  
  
// query function: chuck calls this when loading the Chugin
// NOTE: developer will need to modify this function to
// add additional functions to this Chugin
  CK_DLL_QUERY( Mesh2D )
  {
    // hmm, don't change this...
    QUERY->setname(QUERY, "Mesh2D");
    
    // begin the class definition
    // can change the second argument to extend a different ChucK class
    QUERY->begin_class(QUERY, "Mesh2D", "UGen");
    QUERY->doc_class(QUERY, "This class implements a rectilinear, "
        "two-dimensional digital waveguide mesh "
        "structure.  For details, see Van Duyne and "
        "Smith, &quot;Physical Modeling with the 2-D Digital "
        "Waveguide Mesh,&quot; <i>Proceedings of the 1993 "
        "International Computer Music Conference</i>. "
        "<br/><br/>"
        "This is a digital waveguide model, making its "
        "use possibly subject to patents held by Stanford "
        "University, Yamaha, and others. ");
    QUERY->add_ex(QUERY, "special/Mesh2D.ck");
    
    // register the constructor (probably no need to change)
    QUERY->add_ctor(QUERY, mesh2d_ctor);
    // register the destructor (probably no need to change)
    QUERY->add_dtor(QUERY, mesh2d_dtor);
    
    // for UGen's only: add tick function
    QUERY->add_ugen_func(QUERY, mesh2d_tick, NULL, 0, 1);
    
    // NOTE: if this is to be a UGen with more than 1 channel, 
    // e.g., a multichannel UGen -- will need to use add_ugen_funcf()
    // and declare a tickf function using CK_DLL_TICKF
    
    // example of adding setter method
    QUERY->add_mfun(QUERY, mesh2d_noteOn, "float", "noteOn");
    // example of adding argument to the above method
    QUERY->add_arg(QUERY, "float", "arg");
    QUERY->doc_func(QUERY, "Trigger the mesh with given amplitude.");
    
    // example of adding setter method
    QUERY->add_mfun(QUERY, mesh2d_noteOff, "float", "noteOff");
    // example of adding argument to the above method
    QUERY->add_arg(QUERY, "float", "arg");
    QUERY->doc_func(QUERY, "Release the model.");
    
    // example of adding getter method
    QUERY->add_mfun(QUERY, mesh2d_getNX, "int", "x");
    QUERY->doc_func(QUERY, "X dimension of &quot;plate&quot; (max 12).");
    // example of adding setter method
    QUERY->add_mfun(QUERY, mesh2d_setNX, "int", "x");
    // example of adding argument to the above method
    QUERY->add_arg(QUERY, "int", "arg");
    QUERY->doc_func(QUERY, "X dimension of &quot;plate&quot; (max 12).");
    
    // example of adding getter method
    QUERY->add_mfun(QUERY, mesh2d_getNY, "int", "y");
    QUERY->doc_func(QUERY, "Y dimension of &quot;plate&quot; (max 12).");
    // example of adding setter method
    QUERY->add_mfun(QUERY, mesh2d_setNY, "int", "y");
    // example of adding argument to the above method
    QUERY->add_arg(QUERY, "int", "arg");
    QUERY->doc_func(QUERY, "Y dimension of &quot;plate&quot; (max 12).");
    
    // example of adding getter method
    QUERY->add_mfun(QUERY, mesh2d_getXpos, "float", "xpos");
    QUERY->doc_func(QUERY, "Strike x position (appears to be on range (0.0, 2.0)).");
    // example of adding setter method
    QUERY->add_mfun(QUERY, mesh2d_setXpos, "float", "xpos");
    // example of adding argument to the above method
    QUERY->add_arg(QUERY, "float", "arg");
    QUERY->doc_func(QUERY, "Strike x position (appears to be on range (0.0, 2.0)).");
    
    // example of adding getter method
    QUERY->add_mfun(QUERY, mesh2d_getYpos, "float", "ypos");
    QUERY->doc_func(QUERY, "Strike y position (appears to be on range (0.0, 2.0)).");
    // example of adding setter method
    QUERY->add_mfun(QUERY, mesh2d_setYpos, "float", "ypos");
    // example of adding argument to the above method
    QUERY->add_arg(QUERY, "float", "arg");
    QUERY->doc_func(QUERY, "Strike y position (appears to be on range (0.0, 2.0)).");
    
    // example of adding getter method
    QUERY->add_mfun(QUERY, mesh2d_getDecay, "float", "decay");
    QUERY->doc_func(QUERY, "Decay constant (0 = instant decay, 1.0 = long decay time).");
    // example of adding setter method
    QUERY->add_mfun(QUERY, mesh2d_setDecay, "float", "decay");
    // example of adding argument to the above method
    QUERY->add_arg(QUERY, "float", "arg");
    QUERY->doc_func(QUERY, "Decay constant (0 = instant decay, 1.0 = long decay time).");
    
    // this reserves a variable in the ChucK internal class to store 
    // referene to the c++ class we defined above
    mesh2d_data_offset = QUERY->add_mvar(QUERY, "int", "@m2d_data", false);
    
    // end the class definition
    // IMPORTANT: this MUST be called!
    QUERY->end_class(QUERY);
    
    // wasn't that a breeze?
    return TRUE;
  }
  
  
  // implementation for the constructor
  CK_DLL_CTOR(mesh2d_ctor)
  {
    // get the offset where we'll store our internal c++ class pointer
    OBJ_MEMBER_INT(SELF, mesh2d_data_offset) = 0;
    
    // instantiate our internal c++ class representation
    Mesh2D * bcdata = new Mesh2D(API->vm->get_srate(API, SHRED));
    
    // store the pointer in the ChucK object member
    OBJ_MEMBER_INT(SELF, mesh2d_data_offset) = (t_CKINT) bcdata;
  }
  
  
  // implementation for the destructor
  CK_DLL_DTOR(mesh2d_dtor)
  {
    // get our c++ class pointer
    Mesh2D * bcdata = (Mesh2D *) OBJ_MEMBER_INT(SELF, mesh2d_data_offset);
    // check it
    if( bcdata )
      {
	// clean up
	delete bcdata;
	OBJ_MEMBER_INT(SELF, mesh2d_data_offset) = 0;
	bcdata = NULL;
      }
  }
  
  
  // implementation for tick function
  CK_DLL_TICK(mesh2d_tick)
  {
    // get our c++ class pointer
    Mesh2D * c = (Mesh2D *) OBJ_MEMBER_INT(SELF, mesh2d_data_offset);
    
    // invoke our tick function; store in the magical out variable
    if(c) *out = c->tick(in);
    
    // yes
    return TRUE;
  }

  // example implementation for setter
  CK_DLL_MFUN(mesh2d_noteOn)
  {
    // get our c++ class pointer
    Mesh2D * bcdata = (Mesh2D *) OBJ_MEMBER_INT(SELF, mesh2d_data_offset);
    // set the return value
    RETURN->v_float = bcdata->noteOn(GET_NEXT_FLOAT(ARGS));
  }
    

  // example implementation for setter
  CK_DLL_MFUN(mesh2d_noteOff)
  {
    // get our c++ class pointer
    Mesh2D * bcdata = (Mesh2D *) OBJ_MEMBER_INT(SELF, mesh2d_data_offset);
    // set the return value
    RETURN->v_float = bcdata->noteOff(GET_NEXT_FLOAT(ARGS));
  }

  // example implementation for setter
  CK_DLL_MFUN(mesh2d_setNX)
  {
    // get our c++ class pointer
    Mesh2D * bcdata = (Mesh2D *) OBJ_MEMBER_INT(SELF, mesh2d_data_offset);
    // set the return value
    RETURN->v_int = bcdata->setNX(GET_NEXT_INT(ARGS));
  }
    
  // example implementation for getter
  CK_DLL_MFUN(mesh2d_getNX)
  {
    // get our c++ class pointer
    Mesh2D * bcdata = (Mesh2D *) OBJ_MEMBER_INT(SELF, mesh2d_data_offset);
    // set the return value
    RETURN->v_int = bcdata->getNX();
  }

  // example implementation for setter
  CK_DLL_MFUN(mesh2d_setNY)
  {
    // get our c++ class pointer
    Mesh2D * bcdata = (Mesh2D *) OBJ_MEMBER_INT(SELF, mesh2d_data_offset);
    // set the return value
    RETURN->v_int = bcdata->setNY(GET_NEXT_INT(ARGS));
  }
    
  // example implementation for getter
  CK_DLL_MFUN(mesh2d_getNY)
  {
    // get our c++ class pointer
    Mesh2D * bcdata = (Mesh2D *) OBJ_MEMBER_INT(SELF, mesh2d_data_offset);
    // set the return value
    RETURN->v_int = bcdata->getNY();
  }

  // example implementation for setter
  CK_DLL_MFUN(mesh2d_setXpos)
  {
    // get our c++ class pointer
    Mesh2D * bcdata = (Mesh2D *) OBJ_MEMBER_INT(SELF, mesh2d_data_offset);
    // set the return value
    RETURN->v_float = bcdata->setXpos(GET_NEXT_FLOAT(ARGS));
  }
    
  // example implementation for getter
  CK_DLL_MFUN(mesh2d_getXpos)
  {
    // get our c++ class pointer
    Mesh2D * bcdata = (Mesh2D *) OBJ_MEMBER_INT(SELF, mesh2d_data_offset);
    // set the return value
    RETURN->v_float = bcdata->getXpos();
  }

  // example implementation for setter
  CK_DLL_MFUN(mesh2d_setYpos)
  {
    // get our c++ class pointer
    Mesh2D * bcdata = (Mesh2D *) OBJ_MEMBER_INT(SELF, mesh2d_data_offset);
    // set the return value
    RETURN->v_float = bcdata->setYpos(GET_NEXT_FLOAT(ARGS));
  }
    
  // example implementation for getter
  CK_DLL_MFUN(mesh2d_getYpos)
  {
    // get our c++ class pointer
    Mesh2D * bcdata = (Mesh2D *) OBJ_MEMBER_INT(SELF, mesh2d_data_offset);
    // set the return value
    RETURN->v_float = bcdata->getYpos();
  }

  // example implementation for setter
  CK_DLL_MFUN(mesh2d_setDecay)
  {
    // get our c++ class pointer
    Mesh2D * bcdata = (Mesh2D *) OBJ_MEMBER_INT(SELF, mesh2d_data_offset);
    // set the return value
    RETURN->v_float = bcdata->setDecay(GET_NEXT_FLOAT(ARGS));
  }
    
  // example implementation for getter
  CK_DLL_MFUN(mesh2d_getDecay)
  {
    // get our c++ class pointer
    Mesh2D * bcdata = (Mesh2D *) OBJ_MEMBER_INT(SELF, mesh2d_data_offset);
    // set the return value
    RETURN->v_float = bcdata->getDecay();
  }
}
