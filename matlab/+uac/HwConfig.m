classdef HwConfig < uac.Object
  methods
    function this = HwConfig(varargin)
      this@uac.Object(varargin{:});
    end

    function res = functionName(this, fun, owner)
      res = 'uac_HwConfig';
      res = [res '_' fun];
      if this.ptrType == urx.PtrType.SHARED
        res = [res '_shared'];
      elseif this.ptrType == urx.PtrType.WEAK
        res = [res '_weak'];
      else
        res = [res '_raw'];
      end
      if nargin >=3
        if owner
          res = [res '_shared'];
        else
          res = [res '_raw'];
        end
      end
    end

    function clear(this)
      this.libBindingRef.call(this.functionName('clear'), this);
    end

    function res = size(this)
      res = this.libBindingRef.call(this.functionName('size'), this);
    end

    function res = getKey(this, idx)
      res = this.libBindingRef.call(this.functionName('get_key'), this, idx-1);
    end

    function res = getType(this, idx)
      resInt = this.libBindingRef.call(this.functionName('get_type'), this, idx-1);
      enumList = metaclass(uac.HwConfigEnum.uint8).EnumerationMemberList;
      res = uac.HwConfigEnum.(enumList(resInt+1).Name);
    end

    function res = getSize(this, idx)
      res = this.libBindingRef.call(this.functionName('get_value_size'), this, idx-1);
    end

    function res = getVecStringSize(this, idx)
      res = this.libBindingRef.call(this.functionName('get_size_vec_string'), this, idx-1);
    end

    function res = getValueVecString(this, idx, idx2)
      res = this.libBindingRef.call(this.functionName('get_value_vec_string'), this, idx-1, idx2-1);
    end

    function res = getVecStructSize(this, idx)
      res = this.libBindingRef.call(this.functionName('get_size_vec_struct'), this, idx-1);
    end

    function res = getValueVecStruct(this, idx, idx2)
      res = this.libBindingRef.call(this.functionName('get_value_vec_struct'), this, idx-1, idx2-1);
    end

    function res = getValue(this, idx, type)
      if nargin <= 2
        type = this.getType(idx);
      end
      if type == uac.HwConfigEnum.string
        ptr = this.libBindingRef.call(this.functionName('get_value_string'), this, idx-1);
      else
        ptr = this.libBindingRef.call(this.functionName('get_value'), this, idx-1);
      end
      if type == uac.HwConfigEnum.uint8 || type == uac.HwConfigEnum.vec_uint8
        ptr.setdatatype('uint8Ptr', 1, this.getSize(idx));
        res = ptr.Value;
      elseif type == uac.HwConfigEnum.int8 || type == uac.HwConfigEnum.vec_int8
        ptr.setdatatype('int8Ptr', 1, this.getSize(idx));
        res = ptr.Value;
      elseif type == uac.HwConfigEnum.uint16 || type == uac.HwConfigEnum.vec_uint16
        ptr.setdatatype('uint16Ptr', 1, this.getSize(idx));
        res = ptr.Value;
      elseif type == uac.HwConfigEnum.int16 || type == uac.HwConfigEnum.vec_int16
        ptr.setdatatype('int16Ptr', 1, this.getSize(idx));
        res = ptr.Value;
      elseif type == uac.HwConfigEnum.uint32 || type == uac.HwConfigEnum.vec_uint32
        ptr.setdatatype('uint32Ptr', 1, this.getSize(idx));
        res = ptr.Value;
      elseif type == uac.HwConfigEnum.int32 || type == uac.HwConfigEnum.vec_int32
        ptr.setdatatype('int32Ptr', 1, this.getSize(idx));
        res = ptr.Value;
      elseif type == uac.HwConfigEnum.uint64 || type == uac.HwConfigEnum.vec_uint64
        ptr.setdatatype('uint64Ptr', 1, this.getSize(idx));
        res = ptr.Value;
      elseif type == uac.HwConfigEnum.int64 || type == uac.HwConfigEnum.vec_int64
        ptr.setdatatype('int64Ptr', 1, this.getSize(idx));
        res = ptr.Value;
      elseif type == uac.HwConfigEnum.float || type == uac.HwConfigEnum.vec_float
        ptr.setdatatype('singlePtr', 1, this.getSize(idx));
        res = ptr.Value;
      elseif type == uac.HwConfigEnum.double || type == uac.HwConfigEnum.vec_double
        ptr.setdatatype('doublePtr', 1, this.getSize(idx));
        res = ptr.Value;
      elseif type == uac.HwConfigEnum.string
        res = ptr;
      elseif type == uac.HwConfigEnum.vec_string
        res = {};
        for idx2 = this.getVecStringSize(idx):-1:1
          res{idx2} = this.getValueVecString(idx, idx2);
        end
      elseif type == uac.HwConfigEnum.struct
        subHwConfig = uac.HwConfig(ptr, urx.PtrType.RAW, []);
        res = subHwConfig.fromCpp();
      elseif type == uac.HwConfigEnum.vec_struct
        res = uac.HwConfig.empty;
        for idx2 = this.getVecStructSize(idx):-1:1
          resi = uac.HwConfig(this.getValueVecStruct(idx, idx2), urx.PtrType.RAW, []);
          res{idx2} = resi.fromCpp();
        end
      else
        assert(false);
      end
    end

    function set(this, attribute, type, value)
      enumList = metaclass(type).EnumerationMemberList;
      enumIdx = uint32(find(arrayfun(@(x) strcmp(x.Name, type), enumList))-1);
      functionName = 'set';
      if ischar(value)
        val = libpointer('voidPtr', [uint8(value) 0]);
        size = 1;
      elseif isstruct(value)
        valtmp = uac.HwConfig();
        valtmp.fromStruct(value);
        val = valtmp.id;
        size = 1;
      elseif iscell(value)
        if type == uac.HwConfigEnum.vec_string
          functionName = 'set_vec_string';
          val = value;
          size = numel(value);
        elseif type == uac.HwConfigEnum.vec_struct
          functionName = [];
          size = numel(value);
          for i = size:-1:1
            vali = uac.HwConfig();
            vali.fromStruct(value{i});
            this.libBindingRef.call(this.functionName('append_hwconfig', vali.ptrType == urx.PtrType.SHARED), ...
              this, attribute, vali);
          end
        else
          assert(false);
        end
      else
        val = libpointer([class(value) 'Ptr'], value);
        size = numel(value);
      end
      if ~isempty(functionName)
        this.libBindingRef.call(this.functionName(functionName, isstruct(value)), this.id, ...
          attribute, enumIdx, val, size);
      end
    end

    function fromStruct(this, stru)
      this.clear();
      fields = fieldnames(stru);
      for i = 1:numel(fields)
        fieldName = fields(i);
        if numel(stru.(fieldName{:})) == 1
          prefixEnum = [];
        else
          prefixEnum = 'vec_';
        end

        switch class(stru.(fieldName{:}))
          case {'int8', 'int16', 'int32', 'int64', 'uint8', 'uint16', 'uint32', 'uint64', 'double'}
            typeVal = uac.HwConfigEnum.([prefixEnum class(stru.(fieldName{:}))]);
          case 'struct'
            typeVal = uac.HwConfigEnum.(class(stru.(fieldName{:})));
          case 'single'
            typeVal = uac.HwConfigEnum.([prefixEnum 'float']);
          case 'char'
            typeVal = uac.HwConfigEnum.string;
          case 'cell'
            switch class(stru.(fieldName{:}){1})
              case 'char'
                typeVal = uac.HwConfigEnum.vec_string;
              case 'struct'
                typeVal = uac.HwConfigEnum.vec_struct;
              otherwise
                assert(false);
            end
          otherwise
            assert(false);
        end

        this.set(fieldName{:}, typeVal, stru.(fieldName{:}));
      end
    end

    function res = fromCpp(this)
      size = this.size();  

      if size == 0
        res = struct();
      end
      
      for i = 1:size
        res.(this.getKey(i)) = this.getValue(i);
      end
    end
  end
end
