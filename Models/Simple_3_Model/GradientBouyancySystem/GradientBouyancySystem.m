classdef (StrictDefaults) GradientBouyancySystem < matlab.System
    % Runs Gradient Bouyancy Subsystem
    %
    % NOTE: When renaming the class name untitled7, the file name
    % and constructor name must be updated to use the class name.
    %
    % This template includes most, but not all, possible properties, attributes,
    % and methods that you can implement for a System object in Simulink.

    % Public, tunabxle properties
    properties

    end

    % Public, non-tunable properties
    properties (Nontunable)

    end

    properties (DiscreteState)

    end

    % Pre-computed constants
    properties (Access = private)

    end

    methods
        % Constructor
        function obj = untitled7(varargin)
            % Support name-value pair arguments when constructing object
            setProperties(obj,nargin,varargin{:})
        end
    end

    methods (Access = protected)
        %% Common functions
        function setupImpl(obj)
            % Perform one-time calculations, such as computing constants
        end

        function [force, x_acting, y_acting, z_acting] = stepImpl(obj, x_position, y_position, z_position, roll, pitch, yaw)
            %runs every time
             command = "./GradientBouyancySystem/GradientBouyancy.out " + x_position + " " + y_position + " " + z_position + " " + roll + " " + pitch + " " + yaw;

             [~, cmdout] = system(command);
            outputs = strsplit(cmdout, "|");
            
            X = str2double(outputs);
        
            force = X(1);
            x_acting = X(2);
            y_acting = X(3);
            z_acting = X(4);
        end

        function resetImpl(obj)
            % Initialize / reset discrete-state properties
        end

        %% Backup/restore functions
        function s = saveObjectImpl(obj)
            % Set properties in structure s to values in object obj

            % Set public properties and states
            s = saveObjectImpl@matlab.System(obj);

            % Set private and protected properties
            %s.myproperty = obj.myproperty;
        end

        function loadObjectImpl(obj,s,wasLocked)
            % Set properties in object obj to values in structure s

            % Set private and protected properties
            % obj.myproperty = s.myproperty; 

            % Set public properties and states
            loadObjectImpl@matlab.System(obj,s,wasLocked);
        end

        %% Simulink functions
        function ds = getDiscreteStateImpl(obj)
            % Return structure of properties with DiscreteState attribute
            ds = struct([]);
        end

        function flag = isInputSizeMutableImpl(obj,index)
            % Return false if input size cannot change
            % between calls to the System object
            flag = false;
        end

        function [flag_1, flag_2, flag_3, flag_4] = isOutputFixedSizeImpl(obj)
            flag_1 = true;
            flag_2 = true;
            flag_3 = true;
            flag_4 = true;
        end

        function [out_size_1, out_size_2, out_size_3, out_size_4] = getOutputSizeImpl(obj)
            % Return size for each output port
            out_size_1 = [1 1];
            out_size_2 = [1 1];
            out_size_3 = [1 1];
            out_size_4 = [1 1];

            % Example: inherit size from first input port
            % out = propagatedInputSize(obj,1);
        end

        function [out_type_1, out_type_2, out_type_3, out_type_4] = getOutputDataTypeImpl(obj)
            %return the output type for each port
            out_type_1 = "double";
            out_type_2 = "double";
            out_type_3 = "double";
            out_type_4 = "double";
        end

        function [out_comp_1, out_comp_2, out_comp_3, out_comp_4] = isOutputComplexImpl(obj)
            %return the output type for each port
            out_comp_1 = false;
            out_comp_2 = false;
            out_comp_3 = false;
            out_comp_4 = false;
        end

        function icon = getIconImpl(obj)
            % Define icon for System block
            icon = mfilename("class"); % Use class name
            % icon = "My System"; % Example: text icon
            % icon = ["My","System"]; % Example: multi-line text icon
            % icon = matlab.system.display.Icon("myicon.jpg"); % Example: image file icon
        end
    end

    methods (Static, Access = protected)
        %% Simulink customization functions
        function header = getHeaderImpl
            % Define header panel for System block dialog
            header = matlab.system.display.Header(mfilename("class"));
        end

        function group = getPropertyGroupsImpl
            % Define property section(s) for System block dialog
            group = matlab.system.display.Section(mfilename("class"));
        end
    end
end
