classdef annularextrusion < csg.polyhedron
    
    methods 
    
        function this = annularextrusion (ri, ro, t, segments)

            if nargin < 4
                segments = 20;
            end
            this = this@csg.polyhedron ();

            nodes = [-t/2, ri;
                     -t/2, ro;
                      t/2, ro;
                      t/2, ri; ];

            links = [0;1;2;3;];

            this.make_surface_of_revolution (nodes, links, tau, segments);
            
        end
        
        % Operators
        function r = plus (a, b)
            % call parent class method
            r = plus@csg.polyhedron (a,b);
        end
        
        function r = minus (a, b)
            % call parent class method
            r = minus@csg.polyhedron (a,b);
        end
    
    end


end