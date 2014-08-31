classdef polyhedron < cppinterface
    % base class for computational solid geometry objects
    %
    % Syntax
    %
    % csg.polyhedron()
    % csg.polyhedron(p)
    %
    % Description
    %
    % polyhedron() creates a new polyhedron class without any geometry
    % information.
    %
    % polyhedron(p) creates a new which is a copy of an existing polyhedron
    % (or derived class) providede in 'p'.
    %
    %
    % polyhedron Methods:
    %   makeextrusion
    %   make_surface_of_revolution
    %   makebox
    %   makecylinder
    %   makesphere
    %   makecone
    %   maketorus
    %   render
    %   union
    %   difference
    %   symmetric_difference
    %   translate
    %   rotate
    %   scale
    %   rotmat
    %   transform
    %   num_vertices
    %   num_faces
    %   num_face_vertices
    %   get_vertices
    %   get_vertex
    %   get_face_vertices
    %   triangulate
    %

    methods
    
        % constructor
        function this = polyhedron (other)
        
            this = this@cppinterface (@mexpolyhedron);
            
            if nargin > 0
                
                if isoctave
                    % TODO: when octave implements 'superclasses', use it
                    % for it too
                    if strncmpi (class(other), 'csg', 3)
                        % TODO: what about derived classes?
                        this.cppcall ('copy', other.objectHandle);
                    else
                        error ('Inuput arg was not derived from csg.polyhedron. Could not copy.');
                    end
                else 
                    if isa (other, 'csg.polyhedron') || any (strcmp (superclasses (other), 'csg.polyhedron'))
                        this.cppcall ('copy', other.objectHandle);
                    else
                        error ('Inuput arg was not derived from csg.polyhedron. Could not copy.');
                    end
                end
            end            
            
        end
        
        % info
        function n = num_vertices (this)
        
            n = this.cppcall ('num_vertices');
        
        end
        
        function n = num_face_vertices (this, face_id)
        
            n = this.cppcall ('num_face_vertices', face_id);
        
        end
        
        function n = num_faces (this)
        
            n = this.cppcall ('num_faces');
        
        end
        
        function n = get_vertex (this, vert_id)
        
            n = this.cppcall ('get_vertex', vert_id);
        
        end
        
        function verts = get_face_vertices (this, face_id)
            % returns the indices of the vertices of a face

            verts = this.cppcall ('get_face_vertices', face_id);
            
        end
        
        function verts = get_vertices (this)
            % returns all the vertices from the polyhedron
            
            n = num_vertices (this);
            verts = ones (n,3) * nan;
            for ind = 1:n
                verts(ind,:) =  get_vertex (this, ind-1);
            end
            
        end
        
        % shapes
        function make_extrusion (this, distance, nodes, links)
            % create solid from extruded polygon
            %
            % Syntax
            %
            % polyhedron/make_extrusion  (distance, nodes)
            % polyhedron/make_extrusion  (distance, nodes, links)
            %
            % Input
            %
            %   
            
            if nargin < 4
                % assume nodes are in sequence
                links = 0:size(nodes, 1)-1;
            end
            
            this.cppcall ('extrusion', nodes, links(:), distance);
              
        end
        
        % shapes
        function extrude_rotate (this, distance, segments, dTheta, nodes, links)
            % create solid from extruded polygon
            %
            % Syntax
            %
            % polyhedron/make_extrusion  (distance, segments, dTheta, nodes)
            % polyhedron/make_extrusion  (distance, segments, dTheta, nodes, links)
            %
            % Input
            %
            %   
            
            if nargin < 4
                links = 0:size(nodes, 1)-1;
            end
            
            this.cppcall ('extrude_rotate', nodes, links(:), distance, segments, dTheta);
              
        end
        
        function make_surface_of_revolution (this, nodes, links, angle, segments)
            
            if nargin < 4
                angle = 2 * pi;
            end
            
            if nargin < 5
                segments = 20;
            end
            
            angle = rad2deg (angle);
            
            this.cppcall ('surface_of_revolution', nodes, links, angle, segments);
            
        end
        
        function makebox (this, size_x, size_y, size_z, is_centered)
        
            if nargin < 5
                is_centered = 1;
            end
            
            this.cppcall ('makebox', size_x, size_y, size_z, is_centered);
              
        end
        
        function makesphere (this, radius, is_centered, hsegments, vsegments)
        
            if nargin < 3
                is_centered = 1;
            end
            
            if nargin < 4
                hsegments = 20;
            end
            
            if nargin < 5
                vsegments = 20;
            end
            
            
            this.cppcall ('makesphere', radius, is_centered, hsegments, vsegments);
              
        end

        function makecylinder (this, radius, height, is_centered, segments)
        
            if nargin < 4
                is_centered = 1;
            end
            
            if nargin < 5
                segments = 20;
            end
            
            this.cppcall ('makecylinder', radius, height, is_centered, segments);
              
        end

        function makecone (this, radius, height, is_centered, segments)
        
            if nargin < 4
                is_centered = 1;
            end
            
            if nargin < 5
                hsegments = 20;
            end
            
            this.cppcall ('makecone', radius, height, is_centered, hsegments, vsegments);
              
        end

        function maketorus (this, radius_major, radius_minor, is_centered, major_segments, minor_segments)
        
            if nargin < 4
                is_centered = 1;
            end
            
            if nargin < 5
                major_segments = 20;
            end
            
            if nargin < 6
                minor_segments = 20;
            end
            
            this.cppcall ('makesphere', adius_major, radius_minor, is_centered, major_segments, minor_segments);
              
        end

        
        % modifications
        function translate (this, tvector)
            if ~(isnumeric (tvector)) || (numel(tvector) ~= 3)
                error ('Translation vector must be 3 element numeric translation vector [x, y, z]');
            end
            this.cppcall ('translate', tvector(1), tvector(2), tvector(3));
        end

        function rotate (this, rvector)
            if (~isnumeric (rvector)) || (numel(rvector) ~= 3)
                error ('Rotation vector must be 3 element numeric rotation vector [theta_x, theta_y, theta_z]');
            end
            
            % convert from radians to degrees
            rvector = rad2deg (rvector);
            
            this.cppcall ('rotate', rvector(1), rvector(2), rvector(3));
        
        end
        
        function scale (this, svector)
        
            if isscalar (svector) 
                svector = [svector, svector, svector];
            end
            
            if (~isnumeric (svector)) || (numel(svector) ~= 3)
                error ('Scale vector must be scalar, or a 3 element numeric vector of scales [scale_x, scale_y, scale_z]');
            end
            
            this.cppcall ('scale', svector(1), svector(2), svector(3));
            
        end
        
        function rotmat (this, rotmat)
        
            
            this.cppcall ('rotmat', rotmat(1,1), rotmat(1,2), rotmat(1,3), ...
                                    rotmat(2,1), rotmat(2,2), rotmat(2,3), ...
                                    rotmat(3,1), rotmat(3,2), rotmat(3,3) );
        
        end

        function transform (this, tmat)
            this.cppcall ('transform', tmat(1,1), tmat(1,2), tmat(1,3), tmat(1,4), ...
                                       tmat(2,1), tmat(2,2), tmat(2,3), tmat(2,4), ...
                                       tmat(3,1), tmat(3,2), tmat(3,3), tmat(3,4), ...
                                       tmat(4,1), tmat(4,2), tmat(4,3), tmat(4,4) );
        end
        
        % Boolean operations
        function union (this, other)
        
            this.cppcall ('csgunion', other.objectHandle);
            
        end
        
        function difference (this, other)
        
            this.cppcall ('csgdifference', other.objectHandle);
            
        end
        
        function symmetric_difference (this, other)
        
            this.cppcall ('csgsymmdifference', other.objectHandle);
            
        end
        
        function varargout = triangulate (this)
            % triangulate the surface of the polyhedron, optionally returning
            % all vertices and faces
            %
            % Syntax
            %
            % polyhedron/triangulate (this)
            % [nodes, vertinds] = triangulate (this)
            %
            % 
            
            this.cppcall ('triangulate');
            
            if nargout > 0
                varargout{1} = this.get_vertices ();
                
            end
            
            if nargout > 1
                n = this.num_faces ();
                verts = ones (n,3) * nan;
                for faceind = 1:n
                    verts(faceind,:) = get_face_vertices (this, faceind-1);
                end
                varargout{2} = verts;
            end 
        end
        
        % Operators
        function r = plus (a, b)
        
            % create a deep copy of the first input
            r = csg.polyhedron (a);
            % union it with the second
            r.union (b);
        
        end
        
        function r = minus (a, b)
            
            % create a deep copy of the first input
            r = csg.polyhedron (a);
            % difference it with the second
            r.difference (b);
            
        end
        
        % Drawing
        function render (this, pretty)
            % plot the polyhedron
            %
            % Syntax
            %
            % polyhedron/render ()
            % polyhedron/render (pretty)
            %
            % Input
            %
            %  pretty - optional flag, if true, edges will not be drawn and
            %    lighting added. Default is false
            %
        
            if nargin < 2
                pretty = false;
            end
            
            hfig = figure;
            hax = axes;
            
            n = this.num_faces ();
            
            for face_id = 0:n-1
            
                facecoords = [];
                
                verts = this.get_face_vertices (face_id);
                
                for indii = 1:numel(verts)
                
                    facecoords = [ facecoords; this.get_vertex(verts(indii)) ];
                
                end
                
                if pretty
                    % draw the patch 
                    p = patch ( facecoords(:,1), ...
                                facecoords(:,2), ...
                                facecoords(:,3), 'r');

                    set (p, 'EdgeColor','none');
                    
                else
                    % draw the patch 
                    patch ( facecoords(:,1), ...
                            facecoords(:,2), ...
                            facecoords(:,3), ...
                            'r' );
                end
            
            end
            
            view(3);
            axis equal;
            if pretty, light; end
            xlabel ('x');
            ylabel ('y');
            zlabel ('z');
            
        
        end
        
        
        function stlwrite (this, filename)
            % triangulate and write out polyhedron to stl file
            % 
            % Syntax
            %
            % polyhedron/stlwrite (filename)
            %
            
            [fv.vertices, fv.faces] = this.triangulate ();
            fv.faces = fv.faces + 1;

            csg.stlwrite(filename,fv);
        
        end
        
        function objwrite (this, filename)
            % write the polyhedron to an obj file

            fid = fopen(filename,'w');
            
            % write out the vertices
            nverts = this.num_vertices ();

            for vert = 0:nverts-1
                v = this.get_vertex (vert);
                fprintf(fid,'v %f %f %f\n',v(1),v(2),v(3));
            end

            n = this.num_faces ();
            
            for face_id = 0:n-1
                
                verts = this.get_face_vertices (face_id);
                
                fprintf(fid,'f ');
                for indii = 1:numel(verts)
                    fprintf(fid,'%d ', verts(indii)+1);
                end
                
                fprintf(fid,'\n');
            
            end
            
            fprintf(fid,'g\n');

            fclose(fid);
            
        end
        
        function offwrite (this, filename)
            % write the polyhedron to an OFF file
            
            % open the output file and check for success
            fid = fopen(filename,'w');

            % compute the number of vertices and faces
            nverts = this.num_vertices ();
            nfaces = this.num_faces ();

            % write out the header
            fprintf(fid, 'OFF\n');
            fprintf(fid, '%d %d 0\n', nverts, nfaces );

            % write out the vertices
            for vert = 0:nverts-1
                v = this.get_vertex (vert);
                fprintf(fid,'%f %f %f\n',v(1),v(2),v(3));
            end

            % write out the faces
            for face_id = 0:nfaces-1
                
                verts = this.get_face_vertices (face_id);
                
                fprintf(fid,'%d ', numel (verts));
                for indii = 1:numel(verts)
                    fprintf(fid,'%d ', verts(indii));
                end
                
                fprintf(fid,'\n');
            
            end

            % close the output file
            fclose(fid);
            
        end
        
        function plywrite (this, filename)
            % write the polyhedron to a PLY file
            
            % open the output file and check for success
            fid = fopen(filename,'w');
            

            % compute the number of vertices and faces
            nverts = this.num_vertices ();
            nfaces = this.num_faces ();

            % write out the header
            fprintf(fid, 'ply\n');
            fprintf(fid, 'format ascii 1.0\n');
            fprintf(fid, 'comment ply file gnerated by mpolycsg\n');
            fprintf(fid, 'element vertex %d\n', nverts);
            fprintf(fid, 'property float x\n');
            fprintf(fid, 'property float y\n');
            fprintf(fid, 'property float z\n');
            fprintf(fid, 'element face %d\n', nfaces);
            fprintf(fid, 'property list uchar int vertex_index\n' );
            fprintf(fid, 'end_header\n');

            % write out the vertices
            for vert = 0:nverts-1
                v = this.get_vertex (vert);
                fprintf(fid,'%f %f %f\n',v(1),v(2),v(3));
            end

            % write out the faces
            for face_id = 0:nfaces-1
                
                verts = this.get_face_vertices (face_id);
                
                fprintf(fid,'%d ', numel (verts));
                for indii = 1:numel(verts)
                    fprintf(fid,'%d ', verts(indii));
                end
                
                fprintf(fid,'\n');
            
            end

            % close the output file
            fclose(fid);
            
        end
    
    end

end

    