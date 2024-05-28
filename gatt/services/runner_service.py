class RunnerService:
    
    def __init__(self):
        print("RunnerService initialized")


    def write(self, data, characteristic_uuid):
        print(f"RunnerService received data: {data} to characteristic: {characteristic_uuid}")