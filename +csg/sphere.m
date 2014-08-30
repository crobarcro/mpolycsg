classdef sphere < csg.polyhedron
    
    methods 
    
        function this = sphere (r, varargin)
        
            Inputs.segx = 20;
            Inputs.segy = 20;
            Inputs.Centered = true;
            
            Inputs = parseoptions(Inputs, varargin);

            this = this@csg.polyhedron ();
            
            [X,Y] = pol2cart (linspace (0, pi, Inputs.segx), repmat (r, 1, Inputs.segx));
            
            nodes = [X(:),Y(:)];

            links = (0:numel(X)-1)';

            this.make_surface_of_revolution (nodes, links, tau, Inputs.segy);
            
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