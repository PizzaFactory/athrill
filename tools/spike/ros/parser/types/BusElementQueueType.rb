class BusElementQueueType
    @@objs = Array.new()
    attr_accessor :name
    attr_accessor :dataElmType
    attr_accessor :len

    def initialize(name, dataElmTypeRef, len)
        self.name = name
        self.len = len

        elmName = dataElmTypeRef.split("/")[0]
        type = dataElmTypeRef.split("/")[1]
        if elmName == "DataElementArrayType"
            self.dataElmType = DataElementArrayType.get(type)
        else
            self.dataElmType = DataElementSingleType.get(type)
        end

        BusElementQueueType.register(self)

    end

    def self.add(name, dataElmTypeRef, len)
        BusElementQueueType.new(name, dataElmTypeRef, len)
    end

    def self.register(obj)
        @@objs.push(obj)
    end

    def self.get(name)
        for elm in @@objs do
            if elm.name == name then
                return elm
            end
        end
        p "ERROR: BusElementQueueType(" + name + ") is not found!"
        return nil
    end
end