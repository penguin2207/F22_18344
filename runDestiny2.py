import os
import commands
import csv


LINES = ['1', '2', '4', '8', '16', '32', '64', '128']
#BLOCKSIZE = ['1', '2', '4', '8', '16', '32', '64', '128', '256', '512', '1024', '2048']
#BLOCKSIZE = ['8', '16', '32', '64', '128', '256', '512', '1024', '2048']
BLOCKSIZE = ['8', '16', '32', '64', '128', '256', '512', '1024', '2048', '4096', '8192', '16384', '32768', '65536', '131072', '262144'] #up to 256 B

cacheSize = '32768'

for associativity in LINES:
    for blockSizeBits in BLOCKSIZE:
        config_file = '''-DesignTarget: cache
        -CacheAccessMode: Normal

        -Capacity (B): %s
        -Associativity (for cache only): %s
        -WordWidth (bit): %s


        -ProcessNode: 22

        -DeviceRoadmap: LSTP

        -LocalWireType: LocalAggressive
        -LocalWireRepeaterType: RepeatedNone
        -LocalWireUseLowSwing: No

        -GlobalWireType: GlobalAggressive
        -GlobalWireRepeaterType: RepeatedNone
        -GlobalWireUseLowSwing: No

        -Routing: H-tree
        //-Routing: Non-H-tree

        -InternalSensing: true

        -MemoryCellInputFile: /afs/ece.cmu.edu/class/ece344/opt/destiny/config/sample_SRAM.cell

        -Temperature (K): 350

        //-OptimizationTarget: WriteEDP
        -OptimizationTarget: LeakagePower
        //-OptimizationTarget: Full
        //-OptimizationTarget: ReadLatency
        //-OptimizationTarget: ReadDynamicEnergy
        -EnablePruning: Yes

        -BufferDesignOptimization: latency

        //-ForceBank3D (Total AxBxC, Active DxE): 4x4x4, 1x1
        //-ForceBank (Total AxB, Active CxD): 1x1, 1x1
        //-ForceMat (Total AxB, Active CxD): 1x1, 1x1
        //-ForceMuxSenseAmp: 128
        //-ForceMuxOutputLev1: 1
        //-ForceMuxOutputLev2: 1

        -StackedDieCount: 1
        //-PartitionGranularity: 0
        //-LocalTSVProjection: 0
        //-GlobalTSVProjection: 0
        //-TSVRedundancy: 1.0
        ''' % (cacheSize, associativity, blockSizeBits)
    
        blockSizeByte = int(blockSizeBits)/8
        f = open('myDestiny.cfg', 'w')
        fout = 'c' + cacheSize + '_' + 'a' + associativity + '_' + 'b' + str(blockSizeByte) + '.txt'
        f.write(config_file)
        f.close()

        #os.system('destiny myDestiny.cfg >>' + 'destiny/' + fout)
        status, output = commands.getstatusoutput("destiny myDestiny.cfg")

        areaIdx = output.find('Total Area = ')
        hitIdx = output.find('Hit Latency   = ')
        missIdx = output.find('Miss Latency  = ')
        writeIdx = output.find('Write Latency = ')

        areaVal = output[areaIdx + 13: areaIdx + 13 + 6]
        hitVal = output[hitIdx + 16: hitIdx + 16 + 6]
        missVal = output[missIdx + 16: missIdx + 16 + 6]
        writeVal = output[writeIdx + 16: writeIdx + 16 + 6]
        

        with open('destinyLog.csv', mode='a') as destiny_file:
            destiny_writer = csv.writer(destiny_file, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
            destiny_writer.writerow([cacheSize, associativity, blockSizeByte, areaVal, hitVal, missVal])

            
