/**********************************************************************
generic.h - Handle generic data classes. Custom data for atoms, bonds, etc.
 
Copyright (C) 1998-2001 by OpenEye Scientific Software, Inc.
Some portions Copyright (C) 2001-2005 by Geoffrey R. Hutchison
 
This file is part of the Open Babel project.
For more information, see <http://openbabel.sourceforge.net/>
 
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation version 2 of the License.
 
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
***********************************************************************/

#ifndef OB_GENERIC_H
#define OB_GENERIC_H

#include "babelconfig.h"

#include <string>
#include <vector>

namespace OpenBabel
{

class OBAtom;
class OBBond;
class OBRing;

OBAPI void Trim(std::string& txt);

//! \brief Classification of data stored via OBGenericData class and subclasses.
//!
//! obDataType can be used as a faster, direct access to a particular category
//! instead of the slower access via GetData(std::string). 
//! obData0 through obData15 are data slots that are not used in OpenBabel, and
//! are meant for use in derivative programs.  Macro definitions can be used
//! to define what each data slot is used for.
enum obDataType {obUndefinedData,
		 obPairData,       //!< for arbitrary key/value data
		 obEnergyData,     //!< for energetics data (e.g., total energy, heat of formation, etc.)
                 obCommentData,    //!< for storing text comments
		 obConformerData,  //!< for storing information about conformers
		 obExternalBondData,//!< for storing bond data external to OpenBabel
		 obRotamerList,    //!< for generating & manipulating rotamers
		 obVirtualBondData,//!< for storing bonds to atoms yet to be added
		 obRingData,       //!< for ring data in a molecule
		 obTorsionData,    //!< for torsion/dihedral data
		 obAngleData,      //!< for bond angles in a molecule
		 obSerialNums,     //!< for residue serial numbers
		 obUnitCell,       //!< for crystallographic unit cell data
		 obSpinData,       //!< for spin data, including NMR, atomic and molecular spin, etc.
		 obChargeData,     //!< for partial, total charges, dipole moments, etc.
		 obSymmetryData,   //!< for symmetry data -- point and space groups, transforms, etc.
		 obChiralData,     //!< for arbitrary chiral information (atom, bond, molecule, etc.)
		 obOccupationData, //!< for atomic, molecular occupation data
		 obDensityData,    //!< for density (cube) data
		 obElectronicData, //!< for electronic levels, redox, orbitals, etc.
		 obVibrationData,  //!< for vibrational modes, frequencies, etc.
		 obRotationData,   //!< for rotational energy information
		 obNuclearData,    //!< for nuclear transitions (e.g., decay, fission, fusion)
		 obData0, obData1, obData2, obData3, obData4, obData5,
		 obData6, obData7, obData8, obData9, obData10, obData11,
                 obData12, obData13, obData14, obData15};

//! \brief Base class for generic data
//! 
//! Use obData# slots as an obDataType for custom data types
//! or store your data as a string and use OBPairData for key/value access.
class OBAPI OBGenericData
{
protected:
    std::string     _attr; //!< attribute tag (e.g., "UnitCell", "Comment" or "Author")
    obDataType 	    _type; //!< attribute type -- declared for each subclass
public:
    OBGenericData();
    OBGenericData(const OBGenericData&);
    virtual ~OBGenericData()    {}
    OBGenericData& operator=(const OBGenericData &src);

    void                      SetAttribute(const std::string &v)
    {        _attr = v;        }
    virtual const std::string &GetAttribute()  const
    {        return(_attr);    }
    obDataType                GetDataType()    const
    {        return(_type);    }
};

//! Used to store a comment string (can be multiple lines long)
class OBAPI OBCommentData : public OBGenericData
{
protected:
    std::string _data;
public:
    OBCommentData();
    OBCommentData(const OBCommentData&);
    OBCommentData& operator=(const OBCommentData &src);

    void          SetData(const std::string &data)
    { _data = data; Trim(_data); }
    void          SetData(const char *d)
    {_data = d; Trim(_data);     }
    const std::string &GetData()              const
    {        return(_data);      }
};

//! \brief Used to store information on an external bond 
//! (e.g., SMILES fragments)
class OBAPI OBExternalBond
{
    int     _idx;
    OBAtom *_atom;
    OBBond *_bond;
public:
    OBExternalBond()    {}
    OBExternalBond(OBAtom *,OBBond *,int);
    OBExternalBond(const OBExternalBond &);
    ~OBExternalBond()   {}

    int     GetIdx()  const    {        return(_idx);    }
    OBAtom *GetAtom() const    {        return(_atom);   }
    OBBond *GetBond() const    {        return(_bond);   }
    void SetIdx(int idx)       {        _idx = idx;      }
    void SetAtom(OBAtom *atom) {        _atom = atom;    }
    void SetBond(OBBond *bond) {        _bond = bond;    }
};

//! \brief Used to store information on external bonds (e.g., in SMILES fragments)
class OBAPI OBExternalBondData : public OBGenericData
{
protected:
    std::vector<OBExternalBond> _vexbnd;
public:
    OBExternalBondData();
    void SetData(OBAtom*,OBBond*,int);
    std::vector<OBExternalBond> *GetData()
    {
        return(&_vexbnd);
    }
};

//! Used to store arbitrary attribute/value relationships
class OBAPI OBPairData : public OBGenericData
{
protected:
    std::string _value;
public:
    OBPairData();
    void    SetValue(const char *v)
    {
        _value = v;
    }
    void    SetValue(const std::string &v)
    {
        _value = v;
    }
    std::string &GetValue()
    {
        return(_value);
    }
};

//! \brief Used to temporarily store bonds that reference
//! an atom that has not yet been added to a molecule
class OBAPI OBVirtualBond : public OBGenericData
{
protected:
    int _bgn;
    int _end;
    int _ord;
    int _stereo;
public:
    OBVirtualBond();
    OBVirtualBond(int,int,int,int stereo=0);
    int GetBgn()
    {
        return(_bgn);
    }
    int GetEnd()
    {
        return(_end);
    }
    int GetOrder()
    {
        return(_ord);
    }
    int GetStereo()
    {
        return(_stereo);
    }
};

//! Used to store the SSSR set (filled in by OBMol::GetSSSR())
class OBAPI OBRingData : public OBGenericData
{
protected:
    std::vector<OBRing*> _vr;
public:
    OBRingData();
    OBRingData(const OBRingData &);
    ~OBRingData();

    OBRingData &operator=(const OBRingData &);

    void SetData(std::vector<OBRing*> &vr)
    {
        _vr = vr;
    }
    void PushBack(OBRing *r)
    {
        _vr.push_back(r);
    }
    std::vector<OBRing*> &GetData()
    {
        return(_vr);
    }
};

//! \brief Used for storing information about periodic boundary conditions
//!   with conversion to/from translation vectors and
//!  (a, b, c, alpha, beta, gamma)
class OBAPI OBUnitCell: public OBGenericData
{
protected:
    double _a, _b, _c, _alpha, _beta, _gamma;
    vector3 _offset; //!< offset for origin
    vector3 _v1, _v2, _v3; //!< translation vectors
    std::string _spaceGroup;
public:
    OBUnitCell();
    OBUnitCell(const OBUnitCell &);
    ~OBUnitCell()    {}

    OBUnitCell &operator=(const OBUnitCell &);

    void SetData(const double a, const double b, const double c,
                 const double alpha, const double beta, const double gamma)
    {   _a = a; _b = b; _c = c;
        _alpha = alpha; _beta = beta; _gamma = gamma; }
    void SetData(const vector3 v1, const vector3 v2, const vector3 v3);
    void SetOffset(const vector3 v1) { _offset = v1; }
    //! Set the space group symbol for this unit cell.
    //! Does not create an OBSymmetryData entry or attempt to convert
    //!  between different symbol notations
    void SetSpaceGroup(const std::string sg) { _spaceGroup = sg; }

    double GetA()    { return(_a);    }
    double GetB()    { return(_b);    }
    double GetC()    { return(_c);    }
    double GetAlpha(){ return(_alpha);}
    double GetBeta() { return(_beta); }
    double GetGamma(){ return(_gamma);}
    vector3 GetOffset() { return(_offset); }
    const std::string GetSpaceGroup() { return(_spaceGroup); }

    //! \return v1, v2, v3 cell vectors
    std::vector<vector3> GetCellVectors();
    //! \return v1, v2, v3 cell vectors as a 3x3 matrix
    matrix3x3	GetCellMatrix();
    //! \return The orthogonalization matrix, used for converting from fractional to Cartesian coords.
    matrix3x3 GetOrthoMatrix();
    //! \return The fractionalization matrix, used for converting from Cartesian to fractional coords.
    matrix3x3 GetFractionalMatrix();
};

//! \brief Used to hold data on conformers or geometry optimization steps
class OBAPI OBConformerData: public OBGenericData
{
protected:
  //! Dimensionalities of conformers
  std::vector<unsigned short>              _vDimension;
  //! Relative energies of conformers (preferably in kJ/mol)
  std::vector<double>                      _vEnergies;
  //! Atomic forces for each conformer
  std::vector< std::vector< vector3 > >    _vForces;
  //! Atomic velocities for each conformer (e.g., trajectories)
  std::vector< std::vector< vector3 > >    _vVelocity;
  //! Atomic displacements for each conformer (e.g., RMS distances)
  std::vector< std::vector< vector3 > >    _vDisplace;
  //! Additional data (as strings)
  std::vector<std::string>                 _vData;
    
public:
    OBConformerData();
    OBConformerData(const OBConformerData &);
    ~OBConformerData()    {}

    OBConformerData &operator=(const OBConformerData &);

    void SetDimension(std::vector<unsigned short> vd) { _vDimension = vd; }
    void SetEnergies(std::vector<double> ve) { _vEnergies = ve; }
    void SetForces(std::vector< std::vector< vector3 > > vf) {_vForces = vf;}
    void SetVelocities(std::vector< std::vector< vector3 > > vv)
      { _vVelocity = vv; }
    void SetDisplacements(std::vector< std::vector< vector3 > > vd)
      { _vDisplace = vd; }
    void SetData(std::vector<std::string> vdat) { _vData = vdat; }

    std::vector<unsigned short> GetDimension() { return _vDimension; }
    std::vector<double>         GetEnergies()  { return _vEnergies; }
    std::vector< std::vector< vector3 > > GetForces() {return _vForces; }
    std::vector< std::vector< vector3 > > GetVelocities()
      {return _vVelocity;}
    std::vector< std::vector< vector3 > > GetDisplacements()
      {return _vDisplace;}
    std::vector<std::string>    GetData() { return _vData; }

};

//! \brief Used to hold the point-group and/or space-group symmetry
//! \todo Add support for translation between symbol notations.
//!        Add symmetry perception routines.
class OBAPI OBSymmetryData: public OBGenericData
{
protected:
    std::string _spaceGroup;
    std::string _pointGroup;
public:
    OBSymmetryData();
    OBSymmetryData(const OBSymmetryData &);
    ~OBSymmetryData()    {}

    OBSymmetryData &operator=(const OBSymmetryData &);

    void SetData(std::string pg, std::string sg = "")
      { _pointGroup = pg; _spaceGroup = sg; }
    void SetPointGroup(std::string pg) { _pointGroup = pg; }
    void SetSpaceGroup(std::string sg) { _spaceGroup = sg; }

    std::string GetPointGroup() { return _pointGroup; }
    std::string GetSpaceGroup() { return _spaceGroup; }
};

//! \brief Used to hold the torsion data for a single rotatable bond
//! and all four atoms around it
class OBAPI OBTorsion
{
    friend class OBMol;
    friend class OBTorsionData;

protected:
    std::pair<OBAtom*,OBAtom*> _bc;
    //! double is angle in rads
    std::vector<triple<OBAtom*,OBAtom*,double> > _ads;

    OBTorsion()
    {
        _bc.first=0;
        _bc.second=0;
    }
    ;  //protected for use only by friend classes
    OBTorsion(OBAtom *, OBAtom *, OBAtom *, OBAtom *);

    std::vector<quad<OBAtom*,OBAtom*,OBAtom*,OBAtom*> > GetTorsions();

public:
    OBTorsion(const OBTorsion &);
    ~OBTorsion()
    {}

    OBTorsion& operator=(const OBTorsion &);

    void Clear();
    bool Empty()
    {
        return(_bc.first == 0 && _bc.second == 0);
    }

    bool AddTorsion(OBAtom *a,OBAtom *b, OBAtom *c,OBAtom *d);
    bool AddTorsion(quad<OBAtom*,OBAtom*,OBAtom*,OBAtom*> &atoms);

    bool SetAngle(double radians, unsigned int index = 0);
    bool SetData(OBBond *bond);

    bool GetAngle(double &radians, unsigned int index =0);
    unsigned int GetBondIdx();
    unsigned int GetSize() const
    {
        return (unsigned int)_ads.size();
    }

    std::pair<OBAtom*,OBAtom*>                  GetBC()
    {
        return(_bc);
    }
    std::vector<triple<OBAtom*,OBAtom*,double> > GetADs()
    {
        return(_ads) ;
    }

    bool IsProtonRotor();
};

//! \brief Used to hold torsions as generic data for OBMol.
//! Filled by OBMol::FindTorsions()
class OBAPI OBTorsionData : public OBGenericData
{
    friend class OBMol;

protected:
    std::vector<OBTorsion> _torsions;

    OBTorsionData();
    OBTorsionData(const OBTorsionData &);

public:
    OBTorsionData &operator=(const OBTorsionData &);

    void Clear();

    std::vector<OBTorsion> GetData() const
    {
        return _torsions;
    }
    unsigned int      GetSize() const
    {
        return (unsigned int)_torsions.size();
    }

    void SetData(OBTorsion &torsion);

    bool FillTorsionArray(std::vector<std::vector<unsigned int> > &torsions);
};

//! Used to hold the 3 atoms in an angle and the angle itself
class OBAPI OBAngle
{
    friend class OBMol;
    friend class OBAngleData;

protected:

    //member data

    OBAtom                *_vertex;
    std::pair<OBAtom*,OBAtom*>  _termini;
    double                  _radians;

    //protected member functions

    OBAngle();	//protect constructor for use only by friend classes
    OBAngle(OBAtom *vertex,OBAtom *a,OBAtom *b);

    triple<OBAtom*,OBAtom*,OBAtom*> GetAtoms();
    void SortByIndex();

public:

    OBAngle(const OBAngle &);
    ~OBAngle()
    {
        _vertex = NULL;
    }

    OBAngle &operator = (const OBAngle &);
    bool     operator ==(const OBAngle &);

    void  Clear();

    double GetAngle() const
    {
        return(_radians);
    }

    void  SetAngle(double radians)
    {
        _radians = radians;
    }
    void  SetAtoms(OBAtom *vertex,OBAtom *a,OBAtom *b);
    void  SetAtoms(triple<OBAtom*,OBAtom*,OBAtom*> &atoms);

};


//! \brief Used to hold all angles in a molecule as generic data for OBMol
class OBAPI OBAngleData : public OBGenericData
{
    friend class OBMol;

protected:
    std::vector<OBAngle> _angles;

    OBAngleData();
    OBAngleData(const OBAngleData &);
    std::vector<OBAngle> GetData() const
    {
        return(_angles);
    }

public:
    OBAngleData &operator =(const OBAngleData &);

    void Clear();
    unsigned int FillAngleArray(int **angles, unsigned int &size);
    void         SetData(OBAngle &);
    unsigned int GetSize() const
    {
        return (unsigned int)_angles.size();
    }
};


//****************doxygen for inline functions***********
/*!
**\fn OBTorsionData::GetSize() const
**\brief Gets the number of torsion structs
**\return integer count of the number of torsions
*/

/*!
**\fn OBTorsionData::GetData() const
**\brief Gets a vector of the OBTorsions
**\return the vector of torsions
*/

/*!
**\fn OBAngleData::GetSize() const
**\brief Gets the number of angles 
**\return integer count of the number of angles
*/

/*!
**\fn OBAngleData::GetData() const
**\brief Gets the angle vector data
**\return pointer to vector<OBAngle>
*/

/*!
**\fn OBAngle::SetAngle(double angle)
**\brief Sets the OBAngle angle value
**\param angle in radians
*/

/*!
**\fn OBAngle::GetAngle() const
**\brief Gets the OBAngle angle value
**\return angle in radians
*/

/*!
**\fn OBTorsion::GetBondIdx()
**\brief Gets the bond index of the central bond
**\return int bond index
*/

/*!
**\fn OBTorsion::GetBC()
**\brief Gets the two central atoms of ABCD torsion
**\return pair<OBAtom*,OBAtom*>
*/

/*!
**\fn OBTorsion::GetADs()
**\brief Gets the vector of distal atoms of ABCD torsion
**\return vector of A,D atom pointers and a double
*/

} //end namespace OpenBabel

#endif // OB_GENERIC_H

//! \file generic.h
//! \brief Handle generic data classes. Custom data for atoms, bonds, etc.
