#include <vector>
#include "mex.h"

#define CLASS_HANDLE_SIGNATURE 0xAA01F0A1
#include "class_handle.hpp"

#include "polyhcsg/polyhedron.h"
#include "polyhcsg/polyhedron_binary_op.h"

using namespace polyhcsg;
using namespace mexutils;

// interface to to the polyhedron class from pyPolyCsg
class polyhedron_interface
{
public:
    polyhedron_interface () {}
    
    void copy (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
    {
        // get a pointer to the other polyhedron
        polyhedron* otherph = getotherpoly(nrhs, prhs);
      
        // replace the wrapped poly with a copy of the one supplied
        ph = polyhedron (*otherph);
    }
    
    void extrusion (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
    {
      
        std::vector<double> coords;
        std::vector<int> lines; 
        
        // no input arguemtns expected
        std::vector<int> nallowed;
        nallowed.push_back (3);

        // check the appropriate number of input arguemtns were supplied
        int noffset = mxnarginchk (nrhs, nallowed, 2);
        
        // get the desired vertex id
        mxNumericArrayWrapper wCoords = mxNumericArrayWrapper(prhs[2]);
        mxNumericArrayWrapper wLines = mxNumericArrayWrapper(prhs[3]);
        double distance = mxnthargscalar (nrhs, prhs, 3, 2);
        
        // copy the coordinates matrix
        int m = 0;
        int n = 0;
        std::vector<mwSize> index; index.push_back(0); index.push_back(0);
        for (m = 0; m < wCoords.getRows (); m++)
        {
            for (n = 0; n < 2; n++)
            {
                index[0] = m;
                index[1] = n;
                coords.push_back (wCoords.getDoubleValue(index));
            }  
        }
        
        
        for (m = 0; m < wLines.getRows (); m++)
        {
            for (n = 0; n < 1; n++)
            {
                index[0] = m;
                index[1] = n;
                lines.push_back ((int)(wLines.getDoubleValue(index)));
            }  
        }
        
        ph.initialize_create_extrusion ( coords, lines, distance );
    }
    
    void surface_of_revolution (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
    {
      
        std::vector<double> coords;
        std::vector<int> lines; 
        double angle = 360;
        int segments = 20;
        
        // 3 or 4 input argumetns expected
        std::vector<int> nallowed;
        nallowed.push_back (2);
        nallowed.push_back (3);
        nallowed.push_back (4);

        // check the appropriate number of input arguemtns were supplied
        int noffset = mxnarginchk (nrhs, nallowed, 2);
        
        // get the inputs
        mxNumericArrayWrapper wCoords = mxNumericArrayWrapper(prhs[2]);
        mxNumericArrayWrapper wLines = mxNumericArrayWrapper(prhs[3]);
        
        if (noffset > 2)
        {
            angle = mxnthargscalar (nrhs, prhs, 3, 2);
        }
        
        if (noffset > 3)
        {
             segments = mxnthargscalar (nrhs, prhs, 4, 2);
        }
        
        // copy the coordinates matrix
        int m = 0;
        int n = 0;
        std::vector<mwSize> index; index.push_back(0); index.push_back(0);
        for (m = 0; m < wCoords.getRows (); m++)
        {
            for (n = 0; n < 2; n++)
            {
                index[0] = m;
                index[1] = n;
                coords.push_back (wCoords.getDoubleValue(index));
            }  
        }
        
        for (m = 0; m < wLines.getRows (); m++)
        {
            for (n = 0; n < 1; n++)
            {
                index[0] = m;
                index[1] = n;
                lines.push_back ((int)(wLines.getDoubleValue(index)));
            }  
        }
        
        ph.initialize_create_surface_of_revolution ( coords, lines, angle, segments );
    }
      
    void makebox (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
    {
        double size_x, size_y, size_z;
        bool is_centered;

        // no input arguemtns expected
        std::vector<int> nallowed;
        nallowed.push_back (4);

        // check the appropriate number of input arguemtns were supplied
        mxnarginchk (nrhs, nallowed, 2);
        
        // get the desired vertex id
        size_x = mxnthargscalar (nrhs, prhs, 1, 2);
        size_y = mxnthargscalar (nrhs, prhs, 2, 2);
        size_z = mxnthargscalar (nrhs, prhs, 3, 2);
        is_centered = ( (mxnthargscalar (nrhs, prhs, 4, 2) == 0) ? false : true );
        
        ph.initialize_create_box( size_x, size_y, size_z, is_centered );
    }
    
    void makesphere (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
    {
        double radius = 1.0;
        bool is_centered = false;
        int hsegments = 20; 
        int vsegments = 20;
        int offsetrhs = 0;

        // no input arguemtns expected
        std::vector<int> nallowed;
        nallowed.push_back (2);
        nallowed.push_back (3);
        nallowed.push_back (4);

        // check the appropriate number of input arguemtns were supplied
        offsetrhs = mxnarginchk (nrhs, nallowed, 2);
        
        // get the desired vertex id
        radius = mxnthargscalar (nrhs, prhs, 1, 2);
        
        is_centered = ( (mxnthargscalar (nrhs, prhs, 2, 2) == 0) ? false : true );
        
        if (offsetrhs > 2)
        {
            hsegments = mxnthargscalar (nrhs, prhs, 3, 2);
        }
        
        if (offsetrhs > 3)
        {
            vsegments = mxnthargscalar (nrhs, prhs, 4, 2);
        }
        
        ph.initialize_create_sphere( radius, is_centered, hsegments, vsegments );
        
    }
    
    void makecylinder (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
    {
        double radius = 1.0;
        double height = 1.0;
        bool is_centered = false;
        int segments = 20;
        int offsetrhs = 0;

        // no input arguemtns expected
        std::vector<int> nallowed;
        nallowed.push_back (2);
        nallowed.push_back (3);
        nallowed.push_back (4);

        // check the appropriate number of input arguemtns were supplied
        offsetrhs = mxnarginchk (nrhs, nallowed, 2);
        
        // get the desired vertex id
        radius = mxnthargscalar (nrhs, prhs, 1, 2);
        height = mxnthargscalar (nrhs, prhs, 2, 2);
        
        is_centered = ( (mxnthargscalar (nrhs, prhs, 3, 2) == 0) ? false : true );
        
        if (offsetrhs > 3)
        {
            segments = mxnthargscalar (nrhs, prhs, 4, 2);
        }
        
        ph.initialize_create_cylinder( radius, height, is_centered, segments );
    }
    
    void makecone (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
    {
        double radius = 1.0;
        double height = 1.0;
        bool is_centered = false;
        int segments = 20;
        int offsetrhs = 0;

        // no input arguemtns expected
        std::vector<int> nallowed;
        nallowed.push_back (2);
        nallowed.push_back (3);
        nallowed.push_back (4);

        // check the appropriate number of input arguemtns were supplied
        offsetrhs = mxnarginchk (nrhs, nallowed, 2);
        
        // get the desired vertex id
        radius = mxnthargscalar (nrhs, prhs, 1, 2);
        height = mxnthargscalar (nrhs, prhs, 1, 2);
        
        is_centered = ( (mxnthargscalar (nrhs, prhs, 3, 2) == 0) ? false : true );
        
        if (offsetrhs > 3)
        {
            segments = mxnthargscalar (nrhs, prhs, 3, 2);
        }
        
        ph.initialize_create_cone( radius, height, is_centered, segments );
    }
    
    void maketorus (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
    {
        double radius_major = 1.0;
        double radius_minor = 0.5;
        bool is_centered = false;
        int major_segments = 20;
        int minor_segments = 20;
        int offsetrhs = 0;

        // no input arguemtns expected
        std::vector<int> nallowed;
        nallowed.push_back (3);
        nallowed.push_back (4);
        nallowed.push_back (5);

        // check the appropriate number of input arguemtns were supplied
        offsetrhs = mxnarginchk (nrhs, nallowed, 2);
        
        // get the desired vertex id
        radius_major = mxnthargscalar (nrhs, prhs, 1, 2);
        radius_minor = mxnthargscalar (nrhs, prhs, 2, 2);
        
        is_centered = ( (mxnthargscalar (nrhs, prhs, 3, 2) == 0) ? false : true );
        
        if (offsetrhs > 3)
        {
            major_segments = mxnthargscalar (nrhs, prhs, 4, 2);
        }
        
        if (offsetrhs > 4)
        {
            minor_segments = mxnthargscalar (nrhs, prhs, 5, 2);
        }
        
        ph.initialize_create_torus( radius_major, radius_minor, is_centered, major_segments, minor_segments );
        
    }
      
    
    void num_vertices (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
    {
        // no input arguemtns expected
        std::vector<int> nallowed;
        nallowed.push_back (0);
      
        // check the appropriate number of input arguemtns were supplied
        mxnarginchk (nrhs, nallowed, 2);
      
        int verts = ph.num_vertices ();
      
        // return the number of vertices
        mxSetLHS (verts, 1, nlhs, plhs);
    }

    void num_faces (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
    {
        // no input arguemtns expected
        std::vector<int> nallowed;
        nallowed.push_back (0);
      
        // check the appropriate number of input arguemtns were supplied
        mxnarginchk (nrhs, nallowed, 2);
      
        int faces = ph.num_faces ();
      
        // return the number of vertices
        mxSetLHS (faces, 1, nlhs, plhs);
    }
    
    
    void get_vertex(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
    {
        double x, y, z;
        int id = 0;
        std::vector<int> nallowed;

        // only a single argument is allowed (in addition to class handle
        // arguments)
        nallowed.push_back (1);
        mxnarginchk (nrhs, nallowed, 2);

        // get the desired vertex id
        id = mxnthargscalar (nrhs, prhs, 1, 2);

        // get the vertex from the polyhedron
        ph.get_vertex( id, x, y, z );

        // return it
        double vert[3] = {x,y,z};
        mxSetLHS (vert, 1, 3, nlhs, plhs);

    }
    
    void num_face_vertices(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
    {
        double x, y, z;
        int face_id = 0;
        std::vector<int> nallowed;

        // only a single argument is allowed (in addition to class handle
        // arguments)
        nallowed.push_back (1);
        mxnarginchk (nrhs, nallowed, 2);

        // get the desired vertex id
        face_id = mxnthargscalar (nrhs, prhs, 1, 2);
        
        int nverts = ph.num_face_vertices (face_id);

        // return the number of faces
        mxSetLHS (nverts, 1, nlhs, plhs);
        
    }
    
    
    void get_face_vertices(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
    {
        double x, y, z;
        int face_id = 0;
        int *vertex_id_list;
        std::vector<int> nallowed;

        // only a single argument is allowed (in addition to class handle
        // arguments)
        nallowed.push_back (1);
        mxnarginchk (nrhs, nallowed, 2);

        // get the desired vertex id
        face_id = mxnthargscalar (nrhs, prhs, 1, 2);
        
        int nverts = ph.num_face_vertices (face_id);

        vertex_id_list = new int [nverts];
        
        ph.get_face_vertices( face_id, vertex_id_list );

        // return the list
        mxSetLHS (vertex_id_list, 1, nverts, nlhs, plhs);
        
        delete[] vertex_id_list;
    }
    
    polyhedron* getpolyhedron ()
    {
        return &ph;
    }
    
    void csgunion(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) 
    {
        polyhedron_union union_op;

        // get a pointer to the underlying polyhedron in the other wrapper
        polyhedron* otherph = getotherpoly(nrhs, prhs);
      
        // replace the polyhedron from this obect with the union of it and the
        // other
        ph = union_op (ph, *otherph);
    }
    
    void csgdifference(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) 
    {
        polyhedron_difference diff_op;

        // get a pointer to the underlying polyhedron in the other wrapper
        polyhedron* otherph = getotherpoly(nrhs, prhs);
      
        // replace the polyhedron from this obect with the difference of it and 
        // the other
        ph = diff_op (ph, *otherph);
        
    }
    
    void csgsymmdifference(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
    {
        polyhedron_symmetric_difference symmdiff_op;
      
        // get a pointer to the underlying polyhedron in the other wrapper
        polyhedron* otherph = getotherpoly(nrhs, prhs);
      
        // replace the polyhedron from this obect with the difference of it and 
        // the other
        ph = symmdiff_op (ph, *otherph);
      
    }
    
    void translate(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
    {
        // only a single argument is allowed (in addition to class handle
        // arguments)
        std::vector<int> nallowed;
        nallowed.push_back (3);
        int ofsetrhs = mxnarginchk (nrhs, nallowed, 2);
        
        double x = mxnthargscalar (nrhs, prhs, 1, 2);
        double y = mxnthargscalar (nrhs, prhs, 2, 2);
        double z = mxnthargscalar (nrhs, prhs, 3, 2);
          
        ph = ph.translate( x, y, z );
    }

    void rotate(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
    {
        // only a single argument is allowed (in addition to class handle
        // arguments)
        std::vector<int> nallowed;
        nallowed.push_back (3);
        int ofsetrhs = mxnarginchk (nrhs, nallowed, 2);
        
        double theta_x = mxnthargscalar (nrhs, prhs, 1, 2);
        double theta_y = mxnthargscalar (nrhs, prhs, 2, 2);
        double theta_z = mxnthargscalar (nrhs, prhs, 3, 2);
          
        ph = ph.rotate( theta_x, theta_y, theta_z );
    }
    
    void scale(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
    {
        // three arguments required (in addition to class handle
        // arguments)
        std::vector<int> nallowed;
        nallowed.push_back (3);
        int ofsetrhs = mxnarginchk (nrhs, nallowed, 2);
        
        double x = mxnthargscalar (nrhs, prhs, 1, 2);
        double y = mxnthargscalar (nrhs, prhs, 2, 2);
        double z = mxnthargscalar (nrhs, prhs, 3, 2);
          
        ph = ph.scale( x, y, z );
    }
    
    void rotmat(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
    {
        std::vector<int> nallowed;
        nallowed.push_back (9);
        int ofsetrhs = mxnarginchk (nrhs, nallowed, 2);
      
        // row 1
        double xx = mxnthargscalar (nrhs, prhs, 1, 2);
        double xy = mxnthargscalar (nrhs, prhs, 2, 2);
        double xz = mxnthargscalar (nrhs, prhs, 3, 2);
        // row 2
        double yx = mxnthargscalar (nrhs, prhs, 4, 2);
        double yy = mxnthargscalar (nrhs, prhs, 5, 2);
        double yz = mxnthargscalar (nrhs, prhs, 6, 2);
        // row 3
        double zx = mxnthargscalar (nrhs, prhs, 7, 2);
        double zy = mxnthargscalar (nrhs, prhs, 8, 2);
        double zz = mxnthargscalar (nrhs, prhs, 9, 2);
          
        ph = ph.mult_matrix_3( xx, xy, xz,
                               yx, yy, yz,
                               zx, zy, zz );

    }
    
    void transform(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
    {
        std::vector<int> nallowed;
        nallowed.push_back (16);
        int ofsetrhs = mxnarginchk (nrhs, nallowed, 2);
      
        // row 1
        double xx = mxnthargscalar (nrhs, prhs, 1, 2);
        double xy = mxnthargscalar (nrhs, prhs, 2, 2);
        double xz = mxnthargscalar (nrhs, prhs, 3, 2);
        double xa = mxnthargscalar (nrhs, prhs, 4, 2);
        // row 2
        double yx = mxnthargscalar (nrhs, prhs, 5, 2);
        double yy = mxnthargscalar (nrhs, prhs, 6, 2);
        double yz = mxnthargscalar (nrhs, prhs, 7, 2);
        double ya = mxnthargscalar (nrhs, prhs, 8, 2);
        // row 3
        double zx = mxnthargscalar (nrhs, prhs, 9, 2);
        double zy = mxnthargscalar (nrhs, prhs, 10, 2);
        double zz = mxnthargscalar (nrhs, prhs, 11, 2);
        double za = mxnthargscalar (nrhs, prhs, 12, 2);
        // row 4
        double ax = mxnthargscalar (nrhs, prhs, 13, 2);
        double ay = mxnthargscalar (nrhs, prhs, 14, 2);
        double az = mxnthargscalar (nrhs, prhs, 15, 2);
        double aa = mxnthargscalar (nrhs, prhs, 16, 2);
      
        ph =  ph.mult_matrix_4( xx, xy, xz, xa,
                                yx, yy, yz, ya,
                                zx, zy, zz, za,
                                ax, ay, az, aa );
    }
    
private:

    polyhedron ph;

    polyhedron* getotherpoly(int nrhs, const mxArray *prhs[]) 
    {
        // only a single argument is allowed (in addition to class handle
        // arguments)
        std::vector<int> nallowed;
        nallowed.push_back (1);
        mxnarginchk (nrhs, nallowed, 2);

        // get the pointer to the other nterface class
        polyhedron_interface* otherph_interface = convertMat2Ptr<polyhedron_interface>(prhs[2]);

        // get a pointer to the underlying polyhedron in the other wrapper
        polyhedron* otherph = otherph_interface->getpolyhedron ();
      
        return otherph;
    }
    
    
};

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  
     BEGIN_MEX_CLASS_WRAPPER(polyhedron_interface)
       REGISTER_CLASS_METHOD(polyhedron_interface,copy)
       REGISTER_CLASS_METHOD(polyhedron_interface,num_vertices)
       REGISTER_CLASS_METHOD(polyhedron_interface,num_faces)
       REGISTER_CLASS_METHOD(polyhedron_interface,get_vertex)
       REGISTER_CLASS_METHOD(polyhedron_interface,num_face_vertices)
       REGISTER_CLASS_METHOD(polyhedron_interface,get_face_vertices)
       REGISTER_CLASS_METHOD(polyhedron_interface,csgunion)
       REGISTER_CLASS_METHOD(polyhedron_interface,csgdifference)
       REGISTER_CLASS_METHOD(polyhedron_interface,csgsymmdifference)
       REGISTER_CLASS_METHOD(polyhedron_interface,translate)
       REGISTER_CLASS_METHOD(polyhedron_interface,rotate)
       REGISTER_CLASS_METHOD(polyhedron_interface,scale)
       REGISTER_CLASS_METHOD(polyhedron_interface,rotmat)
       REGISTER_CLASS_METHOD(polyhedron_interface,transform)
       REGISTER_CLASS_METHOD(polyhedron_interface,makebox)
       REGISTER_CLASS_METHOD(polyhedron_interface,makesphere)
       REGISTER_CLASS_METHOD(polyhedron_interface,makecone)
       REGISTER_CLASS_METHOD(polyhedron_interface,maketorus)
       REGISTER_CLASS_METHOD(polyhedron_interface,makecylinder)
       REGISTER_CLASS_METHOD(polyhedron_interface,extrusion)
       REGISTER_CLASS_METHOD(polyhedron_interface,surface_of_revolution)
     END_MEX_CLASS_WRAPPER(polyhedron_interface)


 }
