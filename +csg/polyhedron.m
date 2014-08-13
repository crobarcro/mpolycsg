classdef polyhedron < cppinterface
    % base class for computational solid geometry objects

    methods
    
        % constructor
        function this = polyhedron (other)
        
            this = this@cppinterface (@mexpolyhedron);
            
            if nargin > 0
                
                if isa (other, 'csg.polyhedron')
                    % TODO: what about derived classes?
                    this.cppcall ('copy', other.objectHandle);
                else
                    error ('Inuput arg was not derived from csg.polyhedron. Could not copy.');
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
        
            verts = this.cppcall ('get_face_vertices', face_id);
            
        end
        
        % shapes
        function makeextrusion (this, nodes, links, distance)
            
            this.cppcall ('extrusion', nodes, links, distance);
              
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
                error ('Rotation vector must be 3 element numeric translation vector [theta_x, theta_y, theta_z]');
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
        function render (this)
        
            hfig = figure;
            hax = axes;
            
            n = this.num_faces ();
            
            for face_id = 0:n-1
            
                facecoords = [];
                
                verts = this.get_face_vertices (face_id);
                
                for indii = 1:numel(verts)
                
                    facecoords = [ facecoords; this.get_vertex(verts(indii)) ];
                
                end
                
                % draw the patch 
                patch ( facecoords(:,1), ...
                        facecoords(:,2), ...
                        facecoords(:,3), ...
                        [] );
            
            end
            
            xlabel ('x');
            ylabel ('y');
            zlabel ('z');
        
        end
        
    
    end

end

    