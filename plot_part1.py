#!/usr/bin/env python3
"""
CPSC 457 Assignment 3 - Plotting Script for Part 1
Generates plots for FIFO and Optimal page replacement algorithms
"""

import pandas as pd
import matplotlib.pyplot as plt
import sys
import os

# Set plot style
plt.style.use('seaborn-v0_8-darkgrid')

def load_data():
    """Load CSV data for FIFO and Optimal algorithms"""
    try:
        fifo_df = pd.read_csv('results/data/fifo_results.csv')
        optimal_df = pd.read_csv('results/data/optimal_results.csv')
        return fifo_df, optimal_df
    except FileNotFoundError as e:
        print(f"Error: {e}")
        print("Make sure to run the algorithms first to generate CSV files.")
        sys.exit(1)

def plot_page_faults(fifo_df, optimal_df):
    """
    Plot 1: Page Faults vs Number of Frames
    Two curves: FIFO and Optimal
    """
    plt.figure(figsize=(12, 7))
    
    plt.plot(fifo_df['Frames'], fifo_df['PageFaults'], 
             marker='o', markersize=3, linewidth=2, 
             label='FIFO', color='#E74C3C', linestyle='-')
    
    plt.plot(optimal_df['Frames'], optimal_df['PageFaults'], 
             marker='s', markersize=3, linewidth=2, 
             label='Optimal', color='#3498DB', linestyle='-')
    
    plt.xlabel('Number of Frames', fontsize=14, fontweight='bold')
    plt.ylabel('Number of Page Faults', fontsize=14, fontweight='bold')
    plt.title('Page Faults vs Number of Frames\n(FIFO vs Optimal)', 
              fontsize=16, fontweight='bold', pad=20)
    plt.legend(fontsize=12, loc='upper right')
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    
    # Save plot
    plt.savefig('results/plots/plot1_page_faults.png', dpi=300, bbox_inches='tight')
    print("✓ Generated: results/plots/plot1_page_faults.png")
    plt.close()

def plot_write_backs(fifo_df, optimal_df):
    """
    Plot 2: Write-backs vs Number of Frames
    Two curves: FIFO and Optimal
    """
    plt.figure(figsize=(12, 7))
    
    plt.plot(fifo_df['Frames'], fifo_df['WriteBack'], 
             marker='o', markersize=3, linewidth=2, 
             label='FIFO', color='#E74C3C', linestyle='-')
    
    plt.plot(optimal_df['Frames'], optimal_df['WriteBack'], 
             marker='s', markersize=3, linewidth=2, 
             label='Optimal', color='#3498DB', linestyle='-')
    
    plt.xlabel('Number of Frames', fontsize=14, fontweight='bold')
    plt.ylabel('Number of Write-backs', fontsize=14, fontweight='bold')
    plt.title('Write-backs vs Number of Frames\n(FIFO vs Optimal)', 
              fontsize=16, fontweight='bold', pad=20)
    plt.legend(fontsize=12, loc='upper right')
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    
    # Save plot
    plt.savefig('results/plots/plot2_write_backs.png', dpi=300, bbox_inches='tight')
    print("✓ Generated: results/plots/plot2_write_backs.png")
    plt.close()

def plot_combined(fifo_df, optimal_df):
    """
    Plot 3: Combined plot with all four curves
    - FIFO Page Faults
    - FIFO Write-backs
    - Optimal Page Faults
    - Optimal Write-backs
    """
    plt.figure(figsize=(14, 8))
    
    # FIFO curves
    plt.plot(fifo_df['Frames'], fifo_df['PageFaults'], 
             marker='o', markersize=3, linewidth=2, 
             label='FIFO - Page Faults', color='#E74C3C', linestyle='-')
    
    plt.plot(fifo_df['Frames'], fifo_df['WriteBack'], 
             marker='o', markersize=3, linewidth=2, 
             label='FIFO - Write-backs', color='#E74C3C', linestyle='--', alpha=0.7)
    
    # Optimal curves
    plt.plot(optimal_df['Frames'], optimal_df['PageFaults'], 
             marker='s', markersize=3, linewidth=2, 
             label='Optimal - Page Faults', color='#3498DB', linestyle='-')
    
    plt.plot(optimal_df['Frames'], optimal_df['WriteBack'], 
             marker='s', markersize=3, linewidth=2, 
             label='Optimal - Write-backs', color='#3498DB', linestyle='--', alpha=0.7)
    
    plt.xlabel('Number of Frames', fontsize=14, fontweight='bold')
    plt.ylabel('Count (Page Faults and Write-backs)', fontsize=14, fontweight='bold')
    plt.title('Combined Performance Metrics\n(FIFO vs Optimal - Page Faults and Write-backs)', 
              fontsize=16, fontweight='bold', pad=20)
    plt.legend(fontsize=11, loc='upper right', ncol=2)
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    
    # Save plot
    plt.savefig('results/plots/plot3_combined.png', dpi=300, bbox_inches='tight')
    print("✓ Generated: results/plots/plot3_combined.png")
    plt.close()

def main():
    """Main function to generate all Part 1 plots"""
    print("=" * 60)
    print("CPSC 457 Assignment 3 - Part 1 Plotting")
    print("Generating plots for FIFO and Optimal algorithms")
    print("=" * 60)
    print()
    
    # Create plots directory if it doesn't exist
    os.makedirs('results/plots', exist_ok=True)
    
    # Load data
    print("Loading data from CSV files...")
    fifo_df, optimal_df = load_data()
    print(f"✓ Loaded FIFO data: {len(fifo_df)} entries")
    print(f"✓ Loaded Optimal data: {len(optimal_df)} entries")
    print()
    
    # Generate plots
    print("Generating plots...")
    plot_page_faults(fifo_df, optimal_df)
    plot_write_backs(fifo_df, optimal_df)
    plot_combined(fifo_df, optimal_df)
    
    print()
    print("=" * 60)
    print("All Part 1 plots generated successfully!")
    print("=" * 60)
    print()
    print("Generated plots:")
    print("  1. results/plots/plot1_page_faults.png")
    print("  2. results/plots/plot2_write_backs.png")
    print("  3. results/plots/plot3_combined.png")
    print()

if __name__ == "__main__":
    main()
